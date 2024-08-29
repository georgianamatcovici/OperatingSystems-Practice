#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX 4096

void reverse(char *s)
{
int i, j;
char aux;
for(i=0, j=strlen(s)-1; i<=j; i++, j--)
{aux=s[i];
s[i]=s[j];
s[j]=aux;
}
}

void TacFile(int file)
{int dim;
int pos, cnt;
char ch, buff[MAX];
int r_code;
    dim=lseek(file, 0, SEEK_END);
pos=dim-1;
cnt=0;
while(pos>=0)
{
    lseek(file, pos, SEEK_SET);
    r_code=read(file, &ch, 1);
    if(r_code==-1)
    {printf("Eroare la citire!");
    exit(4);
    }
    pos--;
    if(ch!='\n')
    buff[cnt++]=ch;
    else{
    buff[cnt]='\0';
    reverse(buff);
    printf("%s\n", buff);
    cnt=0;

    }

}
 buff[cnt]='\0';
    reverse(buff);
    printf("%s\n", buff);

}


int main (int argc, char *argv[])
{
int file;
if(argc<2)
{
printf("Utilizare: numele unor fisiere");
exit(1);
}

for(int i=1; i<argc; i++)
{

if(access(argv[i], R_OK)==-1)
{
printf("Nu aveti drept de citire!");
exit(2);
}
file=open(argv[i], O_RDONLY);
if(file==-1)
{
printf("Eroare la deschiderea fisierului!");
exit(3);
}

TacFile(file);
//printf("\n");
}

return 0;
}

