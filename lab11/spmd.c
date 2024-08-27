#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#define MAX 200

int main(int argc, char *argv[])
{
    int i, pid;
    int n, status;
    char param[MAX][MAX], name[MAX][MAX];
    if (argc == 1)
    {
        printf("Nu ati specifficat programul");
        return 1;
    }
    if (argc < 3)
    {
        printf("Nu ati specifficat argumentele");
        return 2;
    }
    
    n = atoi(argv[2]);
    printf(" %d", n);
    for (i = 3; i < argc; i++)
    {
        strcpy(param[i - 3], argv[i]);
        sprintf(name[i - 3], "%s #%d", argv[1], i);
    }

    for (i = 1; i <= n; i++)
    {
        if (-1 == (pid = fork()))
        {
            perror("Eroare la fork");
            exit(2);
        }
        if (0 == pid)
        {
            execlp(argv[1], name[i - 1], param[i - 1], NULL);
            return 10;
        }
    }

    for (i = 1; i <= n; i++)
    {
        wait(&status);
        if (WIFEXITED(status))
        {
            switch (WEXITSTATUS(status))
            {
            case 10:
                printf("Comanda %d nu a putut fi executata... (apelul exec a esuat).\n", i);
                break;
            case 0:
                printf("Comanda %d a fost executata cu succes!\n", i);
                break;
            default:
                printf("Comanda %d a fost executata, dar a esuat, terminandu-se cu codul de terminare: %d.\n", i, WEXITSTATUS(status));
            }
        }
        else
        {
            printf("Comanda %d a fost terminata fortat de catre semnalul: %d.\n", i, WTERMSIG(status));
        }
    }

    return 0;
}