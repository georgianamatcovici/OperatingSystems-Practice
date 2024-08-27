#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

char ch = ' ';
char buf[200];
int received = 0;

void handler_SIGUSR1(int nr_sem)
{
    // if (!received)
    // {
    //     printf("S-a terminat executia procesului.\n");
    //     exit(0);
    // }
    received = 1;
}

void handler_SIGCHLD(int nr_sem)
{
    printf("Tatal: sfarsit executie!\n");
    exit(0);
}

void handler_SIGUSR2(int nr_sem)
{

    printf("Fiul:am primit USR1!.\n");
    fflush(stdout);
    printf("\n%s\n", buf);
    fflush(stdout);
    printf("Fiul:sfarsit executie.\n");
    fflush(stdout);
    exit(0);
}

void handler_SIGINT(int nr_sem)
{
    {
        printf("S-a terminat executia procesului.\n");
        exit(0);
    }
}

void handle_sigalrm(int nr_sem)
{
    if (!received)
        printf("Reminder: trebuie sa introduceti un caracter!\n");
}

int main(int argc, char *argv[])
{
    int pid;
    if (argc < 3)
    {
        perror("Numar insuficient de argumente");
        exit(2);
    }
    int fis_fd;
    fis_fd = open(argv[1], O_WRONLY);
    if (fis_fd == -1)
    {
        perror("Eroare deschidere");
        exit(2);
    }
    int n = atoi(argv[2]);
    signal(SIGUSR1, handler_SIGUSR1);
    signal(SIGUSR2, handler_SIGUSR2);
    // signal(SIGINT, handler_SIGINT);
    signal(SIGCHLD, handler_SIGCHLD);
    signal(SIGALRM, handle_sigalrm);
    pid = fork();
    if (pid == 0)
    {
        int timp_ramas = 5 * n;
        while (timp_ramas > 0)
        {
            alarm(n);
            pause();
            timp_ramas -= n;
        }
        kill(getppid(), SIGUSR1);
    }
    else
    { // parinte
        int it = 0;
        printf("Introduceti un caracter:\n");
        while (read(STDIN_FILENO, &ch, 1) > 0)
        {
            buf[it] = ch;
            it++;
            kill(SIGUSR1, handler_SIGUSR1);
        }
        buf[it] = '\0';
        printf("\n%s\n", buf);
        received = 1;
        kill(pid, SIGUSR2);
        wait(NULL);
    }

    return 0;
}