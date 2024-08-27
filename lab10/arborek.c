#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>


int main(int argc, char *argv[])
{int k, n;
pid_t pid;
int i, j;
int codterm;
if(argc==1)
{
    scanf("Dati k: %d\n", k);
    scanf("Dati n: %d\n", n);
}
else
{k=atoi(argv[1]);
n=atoi(argv[2]);
}

 for(i = 1; i < n; i++)
    {
       for(j=1; j<=k; j++) 
        {if(-1 == (pid=fork()) )
        {
            perror("Eroare la fork");  exit(2);
        }
        }

        if(0 != pid)
        {
          for(j=1; j<=k; j++)
            wait(&codterm); 

            printf("Sunt procesul %d, avand PID-ul: %d, parintele are PID-ul: %d, iar fiul creat are PID-ul: %d si s-a terminat cu codul: %d.\n",
                        i, getpid(), getppid(), pid, codterm>>8 );

            return i; 
        }
        else
        {
            if(i == n-1)  
            {
                printf("Sunt procesul ultim, %d din lantul de procese, avand PID-ul: %d, iar parintele meu este procesul cu PID-ul: %d.\n", n, getpid(), getppid() );
                return n;
            }
        }

    }

    return 0;
}