#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#define MAX 4096

int FLAG_B = 0;
int FLAG_S = 0;
char sep[] = "\n";
void reverse(char *l, char *r)
{
    char aux;
    while (l <= r)
    {
        aux = l[0];
        l[0] = r[0];
        r[0] = aux;
        l++, r--;
    }
}

void funny(char *buff)
{
    char *st = buff;
    char *p = strstr(buff, sep);
    while (p)
    {
        char *newP = strstr(p, sep);
        if (!newP)
            break;
        p = newP + 1;

        reverse(st, newP - 1);
        st = newP;
    }
    if (p)
        reverse(st, buff + strlen(buff) - 1);
}
void TacFile(int file)
{
    int dim;
    int pos, cnt;
    char ch, buff[MAX];
    int r_code;
    dim = lseek(file, 0, SEEK_END);
    pos = dim - 1;
    cnt = 0;
    char *map_addr;
    map_addr=mmap(NULL, dim, PROT_READ, MAP_PRIVATE, file, 0);
    if (map_addr == MAP_FAILED)
        printf("Error at mmap");
    if (-1 == close(file))
        printf("Error at close");
    while (pos >= 0)
    {
        //lseek(file, pos, SEEK_SET);
        ch=*(map_addr+pos);
       // r_code = read(file, &ch, 1);
       // if (r_code == -1)
       // {
       //     printf("Eroare la citire!");
       //     exit(4);
        //}
        pos--;
        buff[cnt] = ch;
        cnt++;
    }

    buff[cnt] = '\0';

    funny(buff);

    if (FLAG_B)
    {
        int nr = 0;
        for (int i = strlen(buff) - 1; i >= 0; i--)
            if (buff[i] != '\n')
                break;
            else
                nr++;
        buff[cnt - nr] = '\0';
        for (int i = 0; i < nr; i++)
            printf("\n");
    }
    printf("%s", buff);
     if (-1 == msync(map_addr, dim, MS_SYNC) )
        printf("Error at msync");
}

int main(int argc, char *argv[])
{
    int file;
    if (argc < 2)
    {
        printf("Utilizare: numele unui fisier");
        exit(1);
    }

    for (int i = 1; i < argc; i++)
    {

        if (strcmp(argv[i], "-s") == 0)
        {
            i++;
            FLAG_S=1;
            strcpy(sep,argv[i]);
            continue;
        }
        else if (strcmp(argv[i], "-b") == 0)
        {
            FLAG_B = 1;
            continue;
        }

        if (access(argv[i], R_OK) == -1)
        {
            printf("Nu aveti drept de citire!");
            exit(2);
        }
        file = open(argv[i], O_RDONLY);
        if (file == -1)
        {
            printf("Eroare la deschiderea fisierului!");
            exit(3);
        }

        TacFile(file);

    }

    return 0;
}