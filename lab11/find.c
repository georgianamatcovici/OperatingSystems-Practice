#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>

int main()
{
    pid_t pid;
    int status;
    if (-1 == (pid = fork()))
    {
        perror("Eroare la fork");
        exit(1);
    }
    if (pid == 0)
    {
        uid_t my_uid;
        struct passwd *st;
        my_uid = getuid();
        st = getpwuid(my_uid);
        // printf("%s\n", st->pw_dir);
        char *arr[] = {"find",
                       st->pw_dir,
                       "(",
                       "-name",
                       "*.c",
                       "-or",
                       "-name",
                       "*.sh",
                       ")",
                       "-printf",
                       "%p : %k KB\n", NULL};
        // + find /home/georgiana '(' -name '*.c' -or -name '*.sh' ')' -printf '%p : %k KB\n'
        execvp("find", arr);
        perror("Eroare la exec");
        exit(10);
    }
    wait(&status);

    if (WIFEXITED(status))
    {
        switch (WEXITSTATUS(status))
        {
        case 10:
            printf("Comanda find nu a putut fi executata... (apelul exec a esuat).\n");
            break;
        case 0:
            printf("Comanda find a fost executata cu succes!\n");
            break;
        default:
            printf("Comanda find a fost executata, dar a esuat, terminandu-se cu codul de terminare: %d.\n", WEXITSTATUS(status));
        }
    }
    else
    {
        printf("Comanda find a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(status));
    }

    return 0;
}