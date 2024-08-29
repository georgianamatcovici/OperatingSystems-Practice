#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#define MAX 500
#include <string.h>

int main(int argc, char *argv[])
{
    char *arg_comm[MAX];
    int i, status;
    if (argc == 1)
    {
        printf("Nu ati specificat comanda");
        exit(1);
    }
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        arg_comm[0] = argv[1];
        for (i = 2; i < argc; i++)
            arg_comm[i - 1] = argv[i];
        arg_comm[i - 1] = NULL;
        execvp(argv[1], arg_comm);
        exit(10);
    }
    else
    {
        wait(&status);

        if (WIFEXITED(status))
        {
            switch (WEXITSTATUS(status))
            {
            case 10:
                printf("Comanda nu a putut fi executata... (apelul exec a esuat).\n");
                break;
            case 0:
                printf("Comanda a fost executata cu succes!\n");
                break;
            default:
                printf("Comanda a fost executata, dar a esuat, terminandu-se cu codul de terminare: %d.\n", WEXITSTATUS(status));
            }
        }
        else
        {
            printf("Comanda a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(status));
        }
    }

    return 0;
}