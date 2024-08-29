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
int fd_fifo;
int fd_pipe[2];
void citire_date()
{
    int cod;
    int nr1, nr2;
    char op;
    int rez;
    printf("Date primite w1: \n");
    do
    {
        cod = read(fd_fifo, &nr1, 4);
        if (cod == -1)
            eroare("eroare read fifo", 6);
        cod = read(fd_fifo, &op, 1);
        if (cod == -1)
            eroare("eroare read fifo", 6);
        cod = read(fd_fifo, &nr2, 4);
        if (cod == -1)
            eroare("eroare read fifo", 6);

        if (cod)
        {
            printf(" %d %c %d\n", nr1, op, nr2);
            switch (op)
            {
            case '+':
            {
                rez = nr1 + nr2;
                break;
            }
            case '-':
            {
                rez = nr1 - nr2;
                break;
            }
            case '*':
            {
                rez = nr1 * nr2;
                break;
            }
            case '/':
            {
                rez = nr1 / nr2;
                break;
            }
            }
            printf("Res: %d\n", rez);
            int cod_w = write(fd_pipe[1], &rez, 4);
            if (cod_w == -1)
                eroare("eroare write pipe", 7);
        }
    } while (cod > 0);
    close(fd_fifo);
}
int main()
{
    if (-1 == mkfifo("fifo_var1", 0600))
    {
        if (errno != EEXIST)
        {
            eroare("eroare fifo", 4);
        }
    }
    fd_fifo = open("fifo_var1", O_RDONLY);
    if (fd_fifo == -1)
        eroare("eroare open fifo", 7);
    if (pipe(fd_pipe) == -1)
        eroare("eroare creare pipe", 5);
    int pid;
    pid = fork();
    if (pid == 0)
    { // fiu
        close(fd_pipe[1]);
        dup2(fd_pipe[0], STDIN_FILENO);
        execlp("/home/georgiana/test_practic2/var1/subordinates/worker2", "worker2", NULL);
        eroare("eroare exec", 5);
    }
    else
    {
        // parinte

        citire_date();
        close(fd_pipe[1]);
    }

    return 0;
}