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
int fd_pipe[2];
int *shm_ptr;
void citire_date(char *path)
{
    int fd_fis;
    char ch;
    fd_fis = open(path, O_RDONLY);
    if (fd_fis == -1)
        handle_error("ERoare deschidere fis", 5);
    int cod;
    do
    {
        cod = read(fd_fis, &ch, 1);
        if (cod == -1)
            handle_error("eroare la citire fis", 7);
        if (cod)
        {
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            {
                // printf("Scriu caracterul: %c\n", ch);
                int cod_w = write(fd_pipe[1], &ch, 1);
                if (cod_w == -1)
                    handle_error("eroare la scriere pipe", 8);
            }
        }
    } while (cod != 0);
}

void finalizare()
{
    sem_t *sem = sem_open("/sem", O_CREAT, 0644, 0);
    sem_wait(sem);
    int nrv, nrc;
    nrv = shm_ptr[0];
    nrc = shm_ptr[1];
    printf("Numerul de vocale: %d\nNumarul de cosoane: %d\n", nrv, nrc);
    sem_unlink("/sem");
    shm_unlink("comunicare_decriptie");
}
int main(int argc, char *argv[])
{
    int pid;
    if (argc < 2)
    {
        handle_error("Nu ati dat fis", 2);
    }
    int shm_fd = shm_open("comunicare_decriptie", O_CREAT | O_RDWR, 0600);
    ftruncate(shm_fd, max_size);
    shm_ptr = mmap(NULL, max_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    close(shm_fd);

    if (-1 == pipe(fd_pipe))
    {
        perror("Eroare la crearea canalului anonim");
        exit(1);
    }
    pid = fork();
    if (pid == -1)
        handle_error("Eroare la fork", 3);
    if (pid == 0)
    { // fiu
        dup2(fd_pipe[0], STDIN_FILENO);
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        execlp("/home/georgiana/test_practic2/var3/slaves/worker1", "worker1", NULL);
        handle_error("eroare la exec", 10);
    }
    else
    {
        // parinte

        citire_date(argv[1]);
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        finalizare();
    }
    return 0;
}
