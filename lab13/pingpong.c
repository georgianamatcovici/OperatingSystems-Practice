#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <signal.h>
#define MAX 200
#define length 4096
#define handle_error(msg, cod) \
    {                          \
        perror(msg);           \
        exit(cod);             \
    }
int fd_fifo1, fd_fifo2, fis_fd;
int main()
{
    if (-1 == mkfifo("fifo_1", 0600))
    {
        if (errno != EEXIST)
        {
            perror("Eroare fifo");
            exit(2);
        }
    }
    if (-1 == mkfifo("fifo_2", 0600))
    {
        if (errno != EEXIST)
        {
            perror("Eroare fifo");
            exit(2);
        }
    }
    fd_fifo1 = open("fifo_1", O_WRONLY);
    int my_pid = getpid();
    write(fd_fifo1, &my_pid, sizeof(int));
    close(fd_fifo1);
    fd_fifo2 = open("fifo_2", O_RDONLY);
    int your_pid;
    read(fd_fifo2, &your_pid, sizeof(int));
    close(fd_fifo2);
    fis_fd = open("fis.txt", O_RDWR);
    while ()

        return 0;
}