#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#define shm_SIZE 4096
#define MAX 200
#define handle_error(msg, cod) \
    {                          \
        perror(msg);           \
        exit(cod);             \
    }
int fd_pipe[2];
int shm_fd;
int *shm_ptr;

void citire_date()
{
    int cod;
    int nr1, nr2;
    int val1, val2, difmax = 0, abs;
    sem_t *sem = sem_open("sem_var2", O_CREAT, 0644, 0);
    if (sem == NULL)
        handle_error("eroare open sem", 8);
    do
    {
        cod = read(fd_pipe[0], &nr1, sizeof(int));
        if (cod == -1)
            handle_error("Eroare la citire din pipe 1", 5);
        cod = read(fd_pipe[0], &nr2, sizeof(int));
        if (cod == -1)
            handle_error("Eroare la citire din pipe 2", 6);
        if (cod)
        {
            printf("Numerele primite de worker2: %d %d\n", nr1, nr2);
            if (nr1 > nr2)
                abs = nr1 - nr2;
            else
                abs = nr2 - nr1;
            if (abs > difmax)
            {
                difmax = abs;
                val1 = nr1;
                val2 = nr2;
            }
        }
        // else
        //    printf("EOF!\n");
    } while (cod);
    printf("Diferenta absoluta maxima: %d\n", difmax);
    shm_ptr[0] = val1;
    shm_ptr[1] = val2;
    sem_post(sem);
    sleep(1);
    // sem_wait(sem);
    if (sem_close(sem) == -1)
        handle_error("eroare close sem", 7);
    sem_unlink("sem_var2");
}

int main()
{
    if (pipe(fd_pipe) == -1)
    {
        handle_error("Eroare la pipe", 2);
    }

    int pid;
    pid = fork();
    if (pid == -1)
        handle_error("Eroare la fork()", 3);
    if (pid == 0)
    { // fiu
        dup2(fd_pipe[1], STDOUT_FILENO);
        close(fd_pipe[1]);
        execlp("/home/georgiana/test_practic2/var2/workers/worker1", "worker1", NULL);
        handle_error("Eroare la exec", 3);
    }
    else
    {
        // parinte
        shm_fd = shm_open("shm_obj_var2", O_CREAT | O_RDWR, 0600);
        if (shm_fd == -1)
            handle_error("eroare open shm", 7);
        if (ftruncate(shm_fd, shm_SIZE) == -1)
            handle_error("ftruncate", 8);
        shm_ptr = mmap(NULL, shm_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
        if (shm_ptr == NULL)
            handle_error("eroare mmap", 7);

        close(fd_pipe[1]);
        citire_date();
        close(shm_fd);
        printf("gata W2");
        fflush(stdout);
        if (shm_unlink("shm_obj_var2") == -1)
            handle_error("eroare unlink shm", 8);

        // printf("Sunt in worker2!\n");
    }

    return 0;
}