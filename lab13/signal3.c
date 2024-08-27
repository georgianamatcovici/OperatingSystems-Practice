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
#include <signal.h>
#define MAX 200
#define length 4096
#define handle_error(msg, cod) \
    {                          \
        perror(msg);           \
        exit(cod);             \
    }
int *map_addr, n;
int pid1, pid2;

void handle2_SIGUSR2(int val)
{
    printf("Fiul2 a primit semnalul\n");
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum = sum ^ map_addr[i];
    }
    printf("Suma XOR: %d\n", sum);
    kill(pid1, SIGUSR1);
}

void handle_SIGUSR2(int val)
{
    printf("Fiul1 a primit semnalul\n");

    int i, j, aux;
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (map_addr[i] < map_addr[j])
            {
                aux = map_addr[i];
                map_addr[i] = map_addr[j];
                map_addr[j] = aux;
            }
    // sigset_t mask;
    // sigfillset(&mask);
    // sigdelset(&mask, SIGUSR1);
    // sigsuspend(&mask);
    printf("Numerele sortate: \n");
    for (i = 0; i < n; i++)
        printf("%d ", map_addr[i]);
    printf("\n");
}

int main(int argc, char *argv[])
{
    map_addr = mmap(NULL,
                    length,
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS,
                    -1,
                    0);
    if (map_addr == MAP_FAILED)
        handle_error("Error at mmap", 2);
    if (argc < 2)
        handle_error("Argumente insuficiente!", 4);
    n = atoi(argv[1]);
    pid1 = fork();
    if (pid1 != 0)
    { // parinte
        pid2 = fork();
        if (pid2 == 0)
        {

            // fiu2->suma
            signal(SIGUSR2, handle2_SIGUSR2);
            sleep(1);
            sigset_t mask;
            sigfillset(&mask);
            sigdelset(&mask, SIGUSR2);
            sigsuspend(&mask);
        }
        else
        {
            srand(time(0));
            int nr;
            for (int i = 0; i < n; i++)
            {
                nr = rand() % 20;
                map_addr[i] = nr;
            }

            kill(pid1, SIGUSR2);
            kill(pid2, SIGUSR2);
        }
    }
    
    else
    {
        // fiu1->sortarea

        signal(SIGUSR2, handle_SIGUSR2);
        sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask, SIGUSR2);
        sigsuspend(&mask);
    }

    if (-1 == munmap(map_addr, length))
        handle_error("Error at munmap", 4);

    return 0;
}