#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>



char *map_addr;
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
			*map_addr=c;
			do{
          c=*map_addr;
			}while(!(c=='0'||c=='2'));
			}
		else i++;
        }
	}
	c='2';
	*map_addr=c;

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
         c=*map_addr;
			}while(!(c=='1'||c=='2'));

		if (fread(&text[i], sizeof(char), 1, f) == 1) {
            if(text[i]=='\n')
			{text[i]='\0';
			printf("%s\n", text);
			i=0;
			c='0';
			*map_addr=c;
		
			}
		else i++;
        }
	}
		c='2';
*map_addr=c;
	fclose(f);
}

int main()
{
	pid_t pid_fiu;
	// TODO: de inițializat cu 0 fișierul flag.bin
    map_addr=mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);  
     if (map_addr == MAP_FAILED)
        printf("Error at mmap");                  

	if(-1 == (pid_fiu=fork()) )
	{
		perror("Eroare la fork");  return 1;
	}

	
	if(pid_fiu == 0)
	{  

		printf("\n[P1] Procesul fiu, cu PID-ul: %d.\n", getpid());
		dialog_fiu();
	}
	else
	{   

		printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());
		dialog_tata();
	}

	printf("Sfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );

	return 0;
}
