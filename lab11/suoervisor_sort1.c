#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 200

int main()
{
    int pid1, pid2, pid3;
    int Cmd1toCmd2[2], Cmd2toCmd3[2], Cmd3toCmd4[2];
    pid1 = fork();
    if (!pid1)
    { // fiu 1
        dup2(Cmd1toCmd2[1], 1);
        execlp("w", "w", "-h", NULL);
        perror("Eroare la execlp() in primul proces fiu. Cauza erorii");
        exit(5);
    }
    else
    {
        pid2 = fork();
        if (!pid2)
        { // fiu 2
            close(Cmd1toCmd2[1]);
            dup2(Cmd1toCmd2[0], 0);
            dup2(Cmd2toCmd3[1], 1);
            execlp("tr", "tr", "-s", " ", NULL);
            perror("Eroare la execlp() in al doilea proces fiu. Cauza erorii");
            exit(6);
        }
        else
        {
            pid3 = fork();
            if (!pid3)
            { // fiu3
                close(Cmd1toCmd2[1]);
                close(Cmd2toCmd3[1]);
                dup2(Cmd2toCmd3[0], 0);
                dup2(Cmd3toCmd4[1], 1);
                execlp("cut", "cut", "-d", "-f1,8", NULL);
                perror("Eroare la execlp() in al treilea proces fiu. Cauza erorii");
                exit(7);
            }
            close(Cmd1toCmd2[1]);
            close(Cmd2toCmd3[1]);
            close(Cmd3toCmd4[1]);
            dup2(Cmd3toCmd4[0], 0);
            execlp("sort", "sort", "-t", " ", "-k", "1", NULL);
            perror("Eroare la execlp() in procesul parinte. Cauza erorii");
            exit(8);
        }
    }
    return 0;
}