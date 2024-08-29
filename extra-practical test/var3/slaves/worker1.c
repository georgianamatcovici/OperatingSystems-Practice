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
#define handle_error(msg, cod) \
    {                          \
        perror(msg);           \
        exit(cod);             \
    }
void procesare()
{
    int cod;
    char ch;
    char new_ch;
    int fifo_fd;
    fifo_fd = open("send_data", O_WRONLY);
    if (-1 == fifo_fd)
        handle_error("eroare deschidere fifo", 7);
    do
    {
        cod = read(STDIN_FILENO, &ch, 1);
        if (cod == -1)
            handle_error("eroare la citire worker1", 2);
        if (cod)
        {
            // printf("Date primite de worker1: %c\n", ch);
            if (ch >= 'a' && ch <= 'z')
            {
                new_ch = (char)((ch - 'a' + 14) % 26 + 'a');
            }
            else
            {
                new_ch = (char)((ch - 'A' + 14) % 26 + 'A');
            }

            int codf = write(fifo_fd, &new_ch, 1);
            if (codf == -1)
                handle_error("eroare scriere fifo", 5);
        }
        // else
        // printf("EOF!");

    } while (cod);
}

int main()
{
    if (-1 == mkfifo("send_data", 0600))
    {
        if (errno != EEXIST)
        {
            handle_error("Eroare mkfifo", 2);
        }
    }
    procesare();

    return 0;
}