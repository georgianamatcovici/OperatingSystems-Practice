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
#define MAX 200
#define shm_SIZE 4096
#define eroare(msg, cod) \
    {                    \
        perror(msg);     \
        exit(cod);       \
    }

FILE *fd_fis;
int fd_fifo;
int *shm_ptr;
void citire_date()
{
    char linie[200], val1[200], val2[100];
    int nr1, nr2;
    char op;
    char *p;
    int cod;
    while (fgets(linie, sizeof(linie), fd_fis))
    {
        p = strchr(linie, ' ');
        val1[0] = 0;
        strncat(val1, linie, p - linie);
        nr1 = atoi(val1);
        op = p[1];
        strcpy(val2, p + 3);
        nr2 = atoi(val2);
        cod = write(fd_fifo, &nr1, sizeof(int));
        if (cod == -1)
            eroare("eroare write fifo", 6);
        cod = write(fd_fifo, &op, sizeof(char));
        if (cod == -1)
            eroare("eroare write fifo", 6);

        cod = write(fd_fifo, &nr2, sizeof(int));
        if (cod == -1)
            eroare("eroare write fifo", 6);
    }
    close(fd_fifo);
}

void finalizare()
{
    int nr1, nr2;
    sem_t *sem = sem_open("sem_var1", O_CREAT, 0644, 0);
    sem_wait(sem);
    nr1 = shm_ptr[0];
    nr2 = shm_ptr[1];
    int sum = nr1 + nr2;
    printf("Suma celor 2 nr: %d", sum);
    sem_close(sem);
    sem_unlink("sem_var1");
}

int main(int argc, char *argv[])
{

    if (argc < 2)
        eroare("arg insuficiente", 3);

    int shm_fd = shm_open("shm_obj_var1", O_CREAT | O_RDWR, 0600);
    if (shm_fd == -1)
        eroare("eroare open shm", 7);
    ftruncate(shm_fd, shm_SIZE);
    shm_ptr = mmap(NULL, shm_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

    if (-1 == mkfifo("fifo_var1", 0600))
    {
        if (errno != EEXIST)
        {
            eroare("eroare fifo", 4);
        }
    }

    fd_fis = fopen(argv[1], "r");
    if (fd_fis == NULL)
        eroare("eroare open fis", 5);
    fd_fifo = open("fifo_var1", O_WRONLY);
    if (fd_fifo == -1)
        eroare("eroare open fifo", 7);
    citire_date();
    finalizare();

    return 0;
}
