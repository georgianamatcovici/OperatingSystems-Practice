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
int *shm_ptr;
void citire_date()
{
    int cod, nr, max_pare, min_imp;
    max_pare = 0;
    min_imp = 1000000;
    printf("Date primite W2: \n");
    do
    {
        cod = read(STDIN_FILENO, &nr, sizeof(int));
        if (cod == -1)
            eroare("eroare read worker2", 7);
        if (cod)
        {
            printf("%d\n", nr);
            if (nr % 2 == 0 && nr > max_pare)
                max_pare = nr;
            if (nr % 2 == 1 && nr < min_imp)
                min_imp = nr;
        }
    } while (cod > 0);
    // printf("Ies din bucla\n");
    // fflush(stdout);
    sem_t *sem = sem_open("sem_var1", O_CREAT, 0644, 0);

    shm_ptr[0] = max_pare;
    shm_ptr[1] = min_imp;
    sem_post(sem);
    sem_close(sem);
    sem_unlink("sem_var1");
}

int main()
{
    int shm_fd = shm_open("shm_obj_var1", O_CREAT | O_RDWR, 0600);
    if (shm_fd == -1)
        eroare("eroare open shm", 3);
    if (ftruncate(shm_fd, shm_SIZE) == -1)
        eroare("ftruncate", 7);
    shm_ptr = mmap(NULL, shm_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == NULL)
        eroare("eroare mmap", 4);
    citire_date();
    if (shm_unlink("shm_obj_var1") == -1)
        eroare("eroare unlink shm", 2);

    return 0;
}