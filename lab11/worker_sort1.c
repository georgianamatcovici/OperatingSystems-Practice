#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#define MAX_MESSAGE 100
#define MAX_PATH 100
#define handle_error(msg, cod)  \
    {                           \
        perror(msg), exit(cod); \
    }
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s /shared_memory_obj_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int shm_fd = shm_open(argv[1], O_RDWR, 0600);
    if (shm_fd == -1)
        handle_error("Error at shm_open", 1);

    char *ptr = mmap(NULL, MAX_MESSAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE, shm_fd, 0);
    if (ptr == MAP_FAILED)
        handle_error("Error at mmap", 2);

    int of, len;
    char path[MAX_PATH];
    memcpy(&of, ptr, sizeof(int));
    memcpy(&len, ptr + sizeof(int), sizeof(int));
    strcpy(path, ptr + 2 * sizeof(int));
    printf("of: %d, len: %d, st: %s\n", of, len, path);

    int file_fd = open(path, O_RDWR);
    if (file_fd == -1)
        handle_error("Error when opening file", 6);

    long page_size = sysconf(_SC_PAGESIZE);

    int offset_in_page = of % page_size;

    of -= offset_in_page;

    char *nptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, file_fd, 0);

    if (nptr == MAP_FAILED)
        handle_error("Error when mmap", 8);

    nptr += offset_in_page;

    int old_bn = 0;
    int bn = 0;
    while (bn < len)
    {
        old_bn = bn;
        while (nptr[bn] != '\n' && bn < len)
            bn++;
        for (int i = old_bn; i < bn; i++)
            for (int j = i + 1; j < bn; j++)
                if (nptr[i] > nptr[j])
                {
                    char aux = nptr[i];
                    nptr[i] = nptr[j];
                    nptr[j] = aux;
                }
        bn++;
    }

    for (int i = bn + 1; i < len; i++)
        for (int j = i + 1; j < len; j++)
            if (nptr[i] > nptr[j])
            {
                char aux = nptr[i];
                nptr[i] = nptr[j];
                nptr[j] = aux;
            }
    nptr -= offset_in_page;

    if (msync(nptr, len, MS_SYNC) == -1)
        handle_error("Error when syncing", 9);
}