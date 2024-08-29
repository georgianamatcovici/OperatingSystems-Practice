#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
int f_inp, f_out;
if(argc<2)
{
printf("Utilizare: fisier1 fisier2!");
exit(1);
}
else if(argc==2)
{
}
else
{
if(access(argv[1], R_OK)==-1)
{
perror("Eroare la acces la fisierul de intrare");
exit(2);
}
if(access(argv[2], W_OK)==-1)
{
perror("Eroare la acces la fisierul de iesire");
exit(3);
}
f_inp=open(argv[1], O_RDONLY);
if(f_inp==-1)
{
perror("Eroare la deschiderea fisierului de intrare!");
exit(4);
}

f_out=open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 00600);
if(f_out==-1)
{
//perror("Eroare la deschiderea fisierului de iesire!");
printf("\nFisierul exista: Doriti sa il suprascrieti? y/n \n");
char r;
scanf("%c", &r);
if(r=='y')
f_out=open(argv[2], O_WRONLY | O_TRUNC, 00600);
else
{
perror("Eroare!");
exit(7);
}
}

int cod_r;
char ch;
while(1)
{
cod_r=read(f_inp, &ch, 1);
if(cod_r==-1)
{perror("Eroare la citire!");
exit(6);}
if(cod_r==0)
break; 
write(f_out, &ch, 1);

}
}

close(f_inp);
close(f_out);
return 0;

}
