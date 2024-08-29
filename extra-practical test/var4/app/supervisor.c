#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#define MAX 200
#define shm_SIZE 4096
#define eroare(msg, cod) \
    {                    \
        perror(msg);     \
        exit(cod);       \
    }
int fd_pipe;
int fd_fis;
int *shm_ptr;

void citire_date()
{
    int cod, codw;
    int ch;

    do
    {
        cod = read(fd_fis, &ch, 1);
        if (cod > 0)
        {
            if (cod == -1)
                eroare("eroare citire fis", 2);
            codw = write(fd_pipe, &ch, 1);
            if (codw == -1)
                eroare("eroare write pipe", 4);
        }
    } while (cod > 0);

    close(fd_pipe);
}

void finalizare()
{
    int nr_voc, nr_cons;
    char voc, cons;
    sem_t *sem = sem_open("sem", O_CREAT, 0644, 0);
    sem_wait(sem);
    voc = (char)shm_ptr[0];
    nr_voc = shm_ptr[1];
    cons = (char)shm_ptr[2];
    nr_cons = shm_ptr[3];
    printf("Numerele sunt: %c %d   %c %d\n", voc, nr_voc, cons, nr_cons);
    sem_unlink("sem");
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        eroare("superviser nu primeste suficiente argumente", 8);
    }
    fd_pipe = atoi(argv[2]);

    fd_fis = open(argv[1], O_RDONLY);
    if (fd_fis == -1)
        eroare("eroare deschidere fis", 2);
    citire_date();
    int shm_fd = shm_open("shm_obj", O_CREAT | O_RDWR, 0600);
    ftruncate(shm_fd, shm_SIZE);
    shm_ptr = mmap(NULL, shm_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    finalizare();
    shm_unlink("shm_obj");
    return 0;
}