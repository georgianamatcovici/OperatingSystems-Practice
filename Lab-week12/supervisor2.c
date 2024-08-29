#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>

int main()
{
    int pid1, pid2;
    int fd1[2], fd2[2];
    if (pipe(fd1) == -1)
    {

        fprintf(stderr, "Pipe error");
        exit(1);
    }
    if (pipe(fd1) == -1)
    {

        fprintf(stderr, "Pipe error");
        exit(2);
    }
    pid1 = fork();
    if (pid1 == 0)
    { // fiu1
        close(fd2[1]);
        char file1[PATH_MAX];
        char ch;
        file1[0] = 0;
        int it = 0;
        int r_code;
        do
        {
            r_code = read(fd1[0], &ch, 1);
            if (r_code != -1 && r_code != 0)
            {

                file1[it] = ch;
                ++it;
            }
        } while (r_code != 0);
        file1[it] = '\0';
        dup2(STDOUT_FILENO, fd1[1]);
        execlp("grep", "grep", "-w", "pipe", file1, NULL);
    }
    else
    {
        pid2 = fork();
        if (pid2 == 0)
        { // fiu2
            close(fd2[1]);
            char file2[PATH_MAX];
            char ch;
            file2[0] = 0;
            int it = 0;
            int r_code;
            do
            {
                r_code = read(fd2[0], &ch, 1);
                if (r_code != -1 && r_code != 0)
                {
                    file2[it] = ch;
                    ++it;
                }
            } while (r_code != 0);
            file2[it] = '\0';
            dup2(STDOUT_FILENO, fd2[1]);
            close(fd2[1]);
            execlp("grep", "grep", "-w", "pipe", file2, NULL);
        }
        else
        { // parinte
            char file1[PATH_MAX], file2[PATH_MAX];
            printf("First file: ");
            fflush(stdout);
            if (1 != scanf("%s", file1))
            {
                fprintf(stderr, "Try again");
                exit(3);
            }
            printf("Second file: ");
            fflush(stdout);
            if (1 != scanf("%s", file2))
            {
                fprintf(stderr, "Try again");
                exit(4);
            }
            if (write(fd1[1], file1, sizeof(file1)) == -1)
            {
                fprintf(stderr, "Writing error");
                exit(3);
            }
            if (write(fd2[1], file2, sizeof(file2)) == -1)
            {
                fprintf(stderr, "Writing error");
                exit(3);
            }
            close(fd1[1]);
            close(fd2[1]);
            int status1, status2;
            waitpid(pid1, &status1, 0);
            int dim1 = 0, dim2 = 0, maxpid;
            maxpid = pid1;
            int r_code;
            do
            {
                char ch;
                r_code = read(fd1[0], &ch, 1);
                if (r_code != 0 && r_code != -1)
                    dim1++;
            } while (r_code != 0);
            waitpid(pid2, &status1, 0);
            do
            {
                char ch;
                r_code = read(fd2[0], &ch, 1);
                if (r_code != 0 && r_code != -1)
                    dim2++;
            } while (r_code != 0);
            if (dim2 > dim1)
                maxpid = pid2;
            printf("More information received from child with pid %d", maxpid);
        }
    }
    return 0;
}