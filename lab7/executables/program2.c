#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
int f1, f2, cod, key2, alter, cod2, key, val, sum, found;
f1=open(argv[1], O_RDWR, 0600);
f2=open(argv[2], O_RDONLY, 0600);

struct flock lacat;

lacat.l_type=F_WRLCK;
lacat.l_whence=SEEK_CUR;
lacat.l_start=-1;
lacat.l_len=sizeof(int);


while(cod=(read(f2, &key2, sizeof(int)))>0)
{
read(f2, &alter, sizeof(int));
found=0;
while(cod2=(read(f1, &key, sizeof(int)))>0 && !found)
{
int r=read(f1, &val, sizeof(int));
if(key==key2)
{
if(-1==fcntl(f1, F_SETLKW, &lacat))
{if(errno==EINTR)
fprintf(stderr, "[ProcesID:%d] Eroare, apelul fcntl a fost intrerupt de un semnal...", getpid());
else
fprintf(stderr,"[ProcesID:%d] Eroare la blocaj...", getpid());
exit(1);
}

lseek(f1, -sizeof(int), SEEK_CUR);
sum=alter+val;
write(f1, &sum, sizeof(int));
found=1;

}
}
if(!found)
{lseek(f1, 0, SEEK_END);
write(f1, &key2, sizeof(int));
write(f1, &alter, sizeof(int));
}

}


close(f1);
close(f2);


    return 0;
}