#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <sys/wait.h>

#define MAX 200
#define MAX_MESSAGE 100
#define handle_error(msg, cod)  \
    {                           \
        perror(msg), exit(cod); \
    }

int main(int argc, char *argv[])
{
    char smh_name[MAX][4096];
    void *ptrs[MAX];
    int fds[MAX];
    if (argc < 2)
    {
        printf("Not enough arguments");
        exit(1);
    }

    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++)
    {
        sprintf(smh_name[i], "shared_memory_obj%d", i);
        fds[i] = shm_open(smh_name[i], O_CREAT | O_RDWR, 0600);
        if (fds[i] == -1)
            handle_error("Error at shm_open", 1);

        if (-1 == ftruncate(fds[i], MAX_MESSAGE))
            handle_error("Error at ftruncate", 2);

        ptrs[i] = mmap(NULL, MAX_MESSAGE, PROT_WRITE | PROT_READ, MAP_SHARED, fds[i], 0);

        if (MAP_FAILED == ptrs[i])
            handle_error("Error at mmap", 3);

        if (-1 == close(fds[i]))
            handle_error("Error at close", 4);
    }

    int file_d = open(argv[2], O_RDWR);
    if (file_d == -1)
        handle_error("Error when opening file", 5);

    int dim = lseek(file_d, 0, SEEK_END);
    if (dim == -1)
        handle_error("Error when lseek-ing", 6);

    char *map = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE, file_d, 0);
    if (map == MAP_FAILED)
    {
        handle_error("Error when mmap-ing", 7);
    }

    int lines = 1;
    for (int i = 0; i < dim; i++)
        if (map[i] == '\n')
            lines++;

    int offset = 0;
    int n_copy = n;
    for (int i = 0; i < n; i++)
    {
        int chunck = lines / (n_copy--);
        lines -= chunck;
        int newOf = offset;
        int count = 0;
        while (count != chunck && newOf < dim)
        {
            if (map[newOf] == '\n')
                count++;
            newOf++;
        }
        memcpy(ptrs[i], &offset, sizeof(int));
        int dif = newOf - offset;
        memcpy(ptrs[i] + sizeof(i), &dif, sizeof(int));
        memcpy(ptrs[i] + 2 * sizeof(int), argv[2], sizeof(argv[2]));
        offset = newOf;
    }
    for (int i = 0; i < n; i++)
    {
        int pid = fork();
        if (pid == -1)
            handle_error("Fork failed", 420);
        if (pid == 0)
        {
            execl("worker_sort1", "Programul worker", smh_name[i], NULL);
        }
    }
    for (int i = 0; i < n; i++)
    {
        int stat = 0;
        int p = wait(&stat);
        if (p == -1)
            handle_error("wait failed", 69);
    }
}