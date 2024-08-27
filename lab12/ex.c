#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#define max_size 4096

#define eroare(mesaj, cod) \
    {                      \
        perror(mesaj);     \
        exit(cod);         \
    }
int pipe1[2];
int pipe2[2];
int main()
{
    int fis_fd;
    int pid1, pid2;
    if (pipe(pipe1) == -1)
        eroare("eroare pipe1", 3);
    if (pipe(pipe2) == -1)
        eroare("eroare pipe2", 3);
    pid1 = fork();
    if (pid1 == 0)
    { // fiu1
        close(pipe2[1]);
        int fd_fifo;
        if (mkfifo("fifo_to_fiu2", 0777) == -1)
        {
            if (errno != EEXIST)
            {
                eroare("Eroare mkfifo", 2);
            }
        }
        fd_fifo = open("fifo_to_fiu2", O_WRONLY);
        if (fd_fifo == -1)
            eroare("eroare open fifo", 8);

        int ch;
        close(pipe1[1]);

        int cod, codw;
        do
        {
            cod = read(pipe1[0], &ch, 1);
            if (cod == -1)
                eroare("eroare read pipe1", 8);
            if (cod > 0)
            {
                if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F'))
                {
                    codw = write(fd_fifo, &ch, 1);
                    if (codw == -1)
                        eroare("eroare write fifo", 8);
                }
                else if (ch >= 'a' && ch <= 'f')
                {
                    int ch2;
                    ch2 = (char)(ch - ('a' - 'A'));
                    codw = write(fd_fifo, &ch2, 1);
                    if (codw == -1)
                        eroare("eroare write fifo", 5);
                }
            }

        } while (cod > 0);
        close(fd_fifo);
    }
    else
    {
        pid2 = fork();

        if (pid2 == 0)
        { // fiu2
            close(pipe1[1]);
            int fd_fifo;

            if (mkfifo("fifo_to_fiu2", 0777) == -1)
            {
                if (errno != EEXIST)
                {
                    eroare("Eroare mkfifo", 2);
                }
            }
            fd_fifo = open("fifo_to_fiu2", O_RDONLY);
            if (fd_fifo == -1)
                eroare("eroare open fifo", 8);
            FILE *fd_fis;
            fd_fis = fopen("statistics.txt", "w");
            if (fd_fis == NULL)
                eroare("eroare open fis", 9);
            int cod, codw;
            int fr[10] = {0};
            char ch;
            do
            {
                cod = read(fd_fifo, &ch, 1);
                if (cod == -1)
                    eroare("eroare citire pipe", 9);
                if (cod > 0)
                {
                    printf("%c\n", ch);
                    if (ch >= '0' && ch <= '9')
                    {
                        fr[ch - '0']++;
                    }
                }
            } while (cod > 0);
            int cnt = 0;
            for (int i = 0; i <= 9; i++)
            {
                char c = (char)(i + '0');
                codw = fprintf(fd_fis, "%c", c);
                if (codw == -1)
                    eroare("eroare write fis", 4);
                codw = fprintf(fd_fis, "%d\n", fr[i]);
                if (codw == -1)
                    eroare("eroare write fis", 4);
                if (fr[i])
                    cnt++;
            }
            codw = write(pipe2[1], &cnt, 4);
            if (codw == -1)
                eroare("eroare write pipe2", 3);
            close(pipe2[1]);
        }
        else
        { // parinte
            char ch;
            fis_fd = open("input_data.txt", O_RDONLY);
            if (fis_fd == -1)
                eroare("eroare open fis", 8);
            close(pipe2[1]);
            int cod;
            do
            {
                cod = read(fis_fd, &ch, 1);
                if (cod == -1)
                    eroare("eroare citire fis", 5);
                if (cod > 0)
                {
                    int codw = write(pipe1[1], &ch, 1);
                    if (codw == -1)
                        eroare("eroare write pipe1", 8);
                }

            } while (cod > 0);
            close(pipe1[1]);
        }
        int nr;
        int cod = read(pipe2[0], &nr, 4);
        if (cod == -1)
            eroare("eroare read in parinte", 8);
        printf("Nr primit: %d\n", nr);
    }

    return 0;
}