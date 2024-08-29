#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>


// 1
// 2 2
// 3 3 3 3 3   3 3 3 3 3
int main()
{ int i;
pid_t pid1, pid2, pid;
int status[10];
int child_pid[10];
pid1=fork();
if(pid1!=0)
{//sunt in 1
wait(NULL);
    pid2=fork();
    
    if(pid2==0)
    { //sunt in 2.2

for(i=1; i<=5; i++)
{
    pid=fork();
    if(pid==0)
    {
        printf("Numar de ordine: (%d, %d), pid-ul procesului: %d, pid-ul parintelui: %d\n", 3, i+5, getpid(), getppid());
        return i+5;
    }
}
for(i=1; i<=5; i++)
{child_pid[i]=wait(&status[i]);
}
printf("Numar de ordine: (%d, %d), pid-ul procesului: %d, pid-ul parintelui: %d\n", 2, 2, getpid(), getppid());
for(i=1; i<=5; i++)
{
printf("  pid fiu #%d: %d, codul acestuia de terminare: %d\n", i, child_pid[i], status[i]>>8);
}
    }
else
{ //sunt in 1
wait(NULL);
wait(NULL);
printf("Numar de ordine: (%d, %d), pid-ul procesului: %d, pid-ul parintelui: %d\n", 1, 1, getpid(), getppid());
}
}
else
{ //sunt in 2.1

for(i=1; i<=5; i++)
{
    pid=fork();
    if(pid==0)
    {
        printf("Numar de ordine: (%d, %d), pid-ul procesului: %d, pid-ul parintelui: %d\n", 3, i, getpid(), getppid());
        return i;
    }
}
for(i=1; i<=5; i++)
{child_pid[i]=wait(&status[i]);
}
printf("Numar de ordine: (%d, %d), pid-ul procesului: %d, pid-ul parintelui: %d\n", 2, 1, getpid(), getppid());
for(i=1; i<=5; i++)
{
printf("  pid fiu #%d: %d, codul acestuia de terminare: %d\n", i, child_pid[i], status[i]>>8);
}
}
        

     


    return 0;
}