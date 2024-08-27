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

int main()
{
    int pid1, pid2;

    return 0;
}