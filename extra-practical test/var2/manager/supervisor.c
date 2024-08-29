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
#define handle_error(msg, cod) \
    {                          \
        perror(msg);           \
        exit(cod);             \
    }

void citire_date(char *path)
{
    FILE *fis_fd = fopen(path, "r");
    if (fis_fd == NULL)
        handle_error("Eroare la deschidere fis", 3);
    char linie[MAX];
    int nr1, nr2;
    char val1[10], val2[10];
    int fifo_fd = open("fifo_var2", O_WRONLY);

    while (fgets(linie, sizeof(linie), fis_fd) > 0)
    {
        char *p = strchr(linie, ' ');
        val1[0] = 0;
        strncat(val1, linie, p - linie);
        nr1 = atoi(val1);
        strcpy(val2, p + 1);
        nr2 = atoi(val2);
        write(fifo_fd, &nr1, sizeof(int));
        write(fifo_fd, &nr2, sizeof(int));
        // printf("Numerele sunt: %d %d\n", nr1, nr2);
    }
    close(fifo_fd);
}

void calc_produs()
{
    sem_t *sem = sem_open("sem_var2", O_CREAT, 0644, 0);
    sem_wait(sem);
    int shm_fd = shm_open("shm_obj_var2", O_CREAT | O_RDWR, 0600);
    // if (shm_fd == -1)
    //     handle_error("eroare shm W2", 7);
    if (ftruncate(shm_fd, shm_SIZE) == -1)
        ;
    // handle_error("eroare ftruncate w2", 9);
    int *shm_ptr = mmap(NULL, shm_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    // if (shm_ptr == NULL)
    //     handle_error("eroare shm mmap w2", 8);
    int nr1, nr2;

    // printf("Sunt inainte de wait()\n");

    nr1 = shm_ptr[0];
    nr2 = shm_ptr[1];
    // sem_post(sem);
    printf("Produsul celor doua numere: %d\n", nr1 * nr2);
    shm_unlink("shm_obj_var2");
    sem_close(sem);
    sem_unlink("sem_var2");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        handle_error("Nu ati dat fisierul", 1);
    }

    if (-1 == mkfifo("fifo_var2", 0600))
    {
        if (errno != EEXIST)
        {
            perror("Eroare la crearea fifo");
            exit(1);
        }
    }

    citire_date(argv[1]);
    calc_produs();
    return 0;
}