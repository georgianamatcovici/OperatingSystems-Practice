#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#define max_size 4096
#define handle_error(msg, cod) \
    {                          \
        perror(msg);           \
        exit(cod);             \
    }

int fifo_fd;
int *shm_ptr;

void procesare()
{

    char ch;
    int cod;
    int nrc = 0, nrv = 0;
    do
    {
        cod = read(fifo_fd, &ch, 1);
        if (cod == -1)
            handle_error("Eroare citire fifo", 7);
        if (cod)
        {
            // printf("Caracterul primit de worker2: %c\n", ch);
            if (strchr("aeiou", ch) != NULL)
                nrv++;
            else
                nrc++;
        }
    } while (cod);
    // printf("NRV, NRC %d, %d\n", nrv, nrc);
    sem_t *sem = sem_open("/sem", O_CREAT, 0644, 0);
    sem_post(sem);
    shm_ptr[0] = nrv;
    shm_ptr[1] = nrc;
    sem_unlink("/sem");
}

int main()
{
    int shm_fd = shm_open("comunicare_decriptie", O_CREAT | O_RDWR, 0600);
    ftruncate(shm_fd, max_size);
    shm_ptr = mmap(NULL, max_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    close(shm_fd);
    if (-1 == mkfifo("send_data", 0600))
    {
        if (errno != EEXIST)
        {
            handle_error("Eroare mkfifo", 2);
        }
    }
    fifo_fd = open("send_data", O_RDONLY);
    if (-1 == fifo_fd)
        handle_error("eroare deschidere fifo", 7);
    procesare();
    shm_unlink("comunicare_decriptie");

    return 0;
}