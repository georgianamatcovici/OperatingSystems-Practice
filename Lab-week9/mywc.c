#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main( int argc, char* argv[])
{
    unsigned long no_bytes, no_words;      
    unsigned int no_newlines, max_length;  
    unsigned char fc,fw,fn,fL;             

    unsigned char last;       

    unsigned int iterator, no_chars_per_line = 0;
    unsigned char ch;
    char *nume_fisier = NULL;
    int fd;

    no_bytes = no_words = no_newlines = max_length = 0;
    last = 0;
    fc = fw = fn = fL = 0;

  
    for (iterator = 1; iterator < argc; iterator++)
    {
        if(0 == strcmp(argv[iterator],"-c")) fc = 1;
        else if(0 == strcmp(argv[iterator],"-w")) fw = 1;
            else if(0 == strcmp(argv[iterator],"-l")) fn = 1;
                else if(0 == strcmp(argv[iterator],"-L")) fL = 1;
                    else if(! nume_fisier) nume_fisier = argv[iterator];
                        else { printf("Programul accepta (deocamdata) un singur nume de fisier, nu mai multe!\n");  exit(1); }
    }
    if( (argc == 1) || (nume_fisier == NULL) )
    {
        fprintf(stderr,"Ati uitat sa specificati numele fisierului de prelucrat!\n");  exit(2);
    }

    if (0 == fc+fw+fn+fL)   
        fc = fw = fn = 1;

   
    if(-1 == (fd = open(nume_fisier, O_RDONLY)))
    {
        perror("Eroare la deschiderea fisierului de prelucrat!\n");  exit(3);
    }
    char *map_addr;

    struct stat s;
    stat(nume_fisier, &s);
    map_addr=mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map_addr == MAP_FAILED)
        printf("Error at mmap");
    if (-1 == close(fd) )
        printf("Error at close");

int it=0;
   while(1)
    {
        ch = *(map_addr+it);
        it++;
        if( it>s.st_size ) break;

       
        no_bytes++;    
        if( fw )
        {
            if( isspace(ch) )
            {
                if(last == 1)  { no_words++; }
                last = 2;
            }
            else
                last = 1;
        }


        if( fn )
        {
            if(ch == '\n') { no_newlines++; }
        }
        if( fL )
        {
            if(ch != '\n')
            {
                if(ch == '\t')
                    no_chars_per_line += 8;  
                else
                    no_chars_per_line++;
            }
            else
            {
                if(max_length < no_chars_per_line) { max_length = no_chars_per_line; }
                no_chars_per_line = 0;
            }
        }
    }

    
   
    if( fw )
    {
        if(last == 1) { no_words++; }
    }

   
    if( fL )
    {
        if(ch != '\n')
        {
            if(max_length < no_chars_per_line) { max_length = no_chars_per_line; }
        }
    }
    printf("Fisierul %s contine: ", nume_fisier);
    if( fc )  printf("%lu caractere (octeti) , ", no_bytes);
    if( fw )  printf("%lu cuvinte , ", no_words);
    if( fn )  printf("%u linii de text (de fapt, newline-uri) , ", no_newlines);
    if( fL )  printf("%u lungimea maxima a liniilor de text.", max_length);
    printf("\n");

    if (-1 == munmap(map_addr, s.st_size) )
        printf("Error at munmap");

    return 0;
}
