#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
    int f_inp, f_out;
    if (argc < 2)
    {
        printf("Utilizare: fisier1 fisier2!");
        exit(1);
    }
    else if (argc == 2)
    {
    }
    else
    {
        if (access(argv[1], R_OK) == -1)
        {
            perror("Eroare la acces la fisierul de intrare");
            exit(2);
        }
        if (access(argv[2], W_OK) == -1)
        {
            perror("Eroare la acces la fisierul de iesire");
            exit(3);
        }
        f_inp = open(argv[1], O_RDONLY);
        if (f_inp == -1)
        {
            perror("Eroare la deschiderea fisierului de intrare!");
            exit(4);
        }

        if (-1 == (f_out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0600)))
        {
            fprintf(stderr, "Eroare la deschiderea write-only a fisierului %s\n", argv[2]);
            perror("Cauza este");
            exit(4);
        }
        char *map_addr1;
        struct stat s;
        stat(argv[1], &s);
        map_addr1 = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, f_inp, 0);
        if (map_addr1 == MAP_FAILED)
            printf("Error at mmap");
        if (-1 == close(f_inp))
            printf("Error at close");

        char *map_addr2;
        if (-1 == ftruncate(f_out, 4096))
            printf("Eroare la trunchierea fisierului de date");
        map_addr2 = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, f_out, 0);
       // printf("%d, %d, %d", map_addr1, map_addr2, map_addr1 - map_addr2);
        if (map_addr2 == MAP_FAILED)
            printf("Error at mmap2");
        if (-1 == close(f_out))
            printf("Error at close");

        int cod_r;
        char ch;
        int index = 0;
        int i1 = 0, i2 = 0;

        char lastCh, lastlastCh;
        char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 +/ ";
        while (1)
        {
            if (i1 > s.st_size)
                break;
            ch = *(map_addr1 + i1);
            i1++;
            index++;
            if (index % 3 == 0 && index != 0)
            {
                char nr1 = char_set[lastlastCh >> 2];
                char nr2 = char_set[((lastlastCh & 0b11) << 4) + (lastCh >> 4)];
                char nr3 = char_set[((lastCh & 0b1111) << 2) + (ch >> 6)];
                char nr4 = char_set[ch & 0b111111];

                // write(f_out, &nr1, 1);
                // write(f_out, &nr2, 1);
                // write(f_out, &nr3, 1);
                // write(f_out, &nr4, 1);
                *(map_addr2 + i2) = nr1;
                i2++;
                *(map_addr2 + i2) = nr2;
                i2++;
                *(map_addr2 + i2) = nr3;
                i2++;
                *(map_addr2 + i2) = nr4;
                i2++;
            }
            lastlastCh = lastCh;
            lastCh = ch;
        }
        if (index % 3 != 0)
        {
            char nr1 = char_set[lastlastCh >> 2];
            char nr2 = char_set[((lastlastCh & 0b11) << 4) + (lastCh >> 4)];
            char nr3 = char_set[((lastCh & 0b1111) << 2) + (ch >> 6)];
            char nr4 = '=';
            if (index % 3 == 1)
                nr3 = '=';
            // write(f_out, &nr1, 1);
            // write(f_out, &nr2, 1);
            // write(f_out, &nr3, 1);
            // write(f_out, &nr4, 1);
            *(map_addr2 + i2) = nr1;
            i2++;
            *(map_addr2 + i2) = nr2;
            i2++;
            *(map_addr2 + i2) = nr3;
            i2++;
            *(map_addr2 + i2) = nr4;
            i2++;
        }

        // 0b1001110
        if (-1 == msync(map_addr2, 4096, MS_SYNC))
            printf("Error at msync");
        //
    }

    return 0;
}