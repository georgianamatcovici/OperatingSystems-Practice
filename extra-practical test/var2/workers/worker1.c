#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#define MAX 200
#define handle_error(msg, cod) \
    {                          \
        perror(msg);           \
        exit(cod);             \
    }

int cmmdc(int a, int b)
{
    int r;
    while (a % b != 0)
    {
        r = a % b;
        a = b;
        b = r;
    }
    return b;
}

void citire_date(int fifo_fd)
{

    int cod;
    int nr1, nr2;
    do
    {
        cod = read(fifo_fd, &nr1, sizeof(int));
        if (cod == -1)
            handle_error("Eroare la citire 1", 1);
        cod = read(fifo_fd, &nr2, sizeof(int));
        if (cod == -1)
            handle_error("Eroare la citire 2", 2);
        if (cod)
        {
            // printf("Numerele primite de worker1: %d %d\n", nr1, nr2);
            if (cmmdc(nr1, nr2) == 1)
            {
                int cod1;
                cod1 = write(STDOUT_FILENO, &nr1, sizeof(int));
                if (cod1 == -1)
                    handle_error("eroare scriere 1", 4);
                cod1 = write(STDOUT_FILENO, &nr2, sizeof(int));
                if (cod1 == -1)
                    handle_error("eroare scriere 2", 5);
            }
        }

    } while (cod);
    close(STDOUT_FILENO);
    close(fifo_fd);
}

int main()
{
    if (-1 == mkfifo("fifo_var2", 0600))
    {
        if (errno != EEXIST)
        {
            perror("Eroare la crearea canalului fifo. Cauza erorii");
            exit(1);
        }
    }
    int fifo_fd = open("fifo_var2", O_RDONLY);
    if (fifo_fd == -1)
        handle_error("eroare open fifo", 7);
    citire_date(fifo_fd);
    close(fifo_fd);

    return 0;
}