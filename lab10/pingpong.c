#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>



int flag_file;
void dialog_tata()
{
	// TODO: de implementat bucla dialogului pentru tata

	 // Scrieți două bucle imbricate, conform indicațiilor cuprinse în diagramă!

  FILE *f;
  char c;
  char text[500];
  int i=0;
   f = fopen("replici-parinte.txt", "r");
    if (f == NULL) {
        printf("Eroare la deschiderea primului fisier");   exit(2);
    }
	while(!feof(f))
	{
		if (fread(&text[i], sizeof(char), 1, f) == 1) {
            if(text[i]=='\n')
			{text[i]='\0';
			printf("%s\n", text);
			i=0;
			c='1';
			write(flag_file, &c, 1);
			lseek(flag_file, 0, SEEK_SET);
			do{
           read(flag_file, &c, 1);
		   lseek(flag_file, 0, SEEK_SET);
			}while(!(c=='0'||c=='2'));
			}
		else i++;
        }
	}
	c='2';
	write(flag_file, &c, 1);
	lseek(flag_file, 0, SEEK_SET);

fclose(f);
}

void dialog_fiu()
{	
  FILE *f;
  char c;
  char text[500];
int i;
i=0;
f = fopen("replici-fiu.txt", "r");
    if (f == NULL) {
        printf("Eroare la deschiderea celui de-al doilea fisier");   exit(2);
    }
	while(!feof(f))
	{
			do{
           read(flag_file, &c, 1);
		   //printf("\n\n%c\n", c);
		   lseek(flag_file, 0, SEEK_SET);
			}while(!(c=='1'||c=='2'));

		if (fread(&text[i], sizeof(char), 1, f) == 1) {
            if(text[i]=='\n')
			{text[i]='\0';
			printf("%s\n", text);
			i=0;
			c='0';
			write(flag_file, &c, 1);
			lseek(flag_file, 0, SEEK_SET);
		
			}
		else i++;
        }
	}
		c='2';
	write(flag_file, &c, 1);
	lseek(flag_file, 0, SEEK_SET);
	fclose(f);
}

int main()
{
	pid_t pid_fiu;
	// TODO: de inițializat cu 0 fișierul flag.bin
	flag_file=open("flag.bin", O_RDWR);
	if(flag_file==-1)
	printf("Eroare la deschiderea flag.bin");
    char c='0';
    write(flag_file, &c, 1);
	lseek(flag_file, 0, SEEK_SET);

	/* Crearea unui proces fiu. */
	if(-1 == (pid_fiu=fork()) )
	{
		perror("Eroare la fork");  return 1;
	}

	/* Ramificarea execuției în cele două procese, tată și fiu. */
	if(pid_fiu == 0)
	{   /* Zona de cod executată doar de către fiu. */

		printf("\n[P1] Procesul fiu, cu PID-ul: %d.\n", getpid());
		dialog_fiu();
	}
	else
	{   /* Zona de cod executată doar de către părinte. */

		printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());
		dialog_tata();
	}

	/* Zona de cod comună, executată de către ambele procese */
	printf("Sfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );
	close(flag_file);
	return 0;
}
