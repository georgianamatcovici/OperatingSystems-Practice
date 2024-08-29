#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s fisier-sursa fisier-destinatie\n", argv[0]);
        exit(1);
    }

    if (-1 == access(argv[1], R_OK))
    {
        fprintf(stderr, "Eroare la access pentru %s .\t", argv[1]);
        perror("Cauza este");
        exit(2);
    }

    int f_sursa, f_dest;

    if (-1 == (f_sursa = open(argv[1], O_RDONLY)))
    {
        fprintf(stderr, "Eroare la deschiderea read-only a fisierului %s\n", argv[1]);
        perror("Cauza este");
        exit(3);
    }

    if (-1 == (f_dest = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0600)))
    {
        fprintf(stderr, "Eroare la deschiderea write-only a fisierului %s\n", argv[2]);
        perror("Cauza este");
        exit(4);
    }

    int val = 0, nr = 0, sum = 0;
    char *map_addr1;

    struct stat s;
    stat(argv[1], &s);
    map_addr1 = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, f_sursa, 0);
    if (map_addr1 == MAP_FAILED)
        printf("Error at mmap");
    if (-1 == close(f_sursa))
        printf("Error at close");

    char *map_addr2;
    if (-1 == ftruncate(f_dest, 4096))
        printf("Eroare la trunchierea fisierului de date");
    map_addr2 = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, f_dest, 0);
    if (map_addr2 == MAP_FAILED)
        printf("Error at mmap2");
    if (-1 == close(f_dest))
        printf("Error at close");
    int it = 0, i = 0;

    sum = 0;
    nr = 0;
    while (1)
    {
        val = 0;
        char c = map_addr1[it];
        while (c <= '9' && c >= '0')
        {
            val = val * 10 + c - '0';
            it++;
            c = map_addr1[it];
        }
        it++;
        // printf("%d\n", val);
        if (it > s.st_size)
            break;

        if (0 != val)
        {
            sum += val;
            nr++;
            // printf("%d %d %d\n", val, sum, nr);
            float ans = (float)sum / nr;
            int funny = (int)(ans * 100);

            int p = 1;
            while (p * 10 <= funny)
                p *= 10;

            // fprintf(f_dest,"%.2f\n",(float)sum/nr);

            while (p)
            {
                if (p == 10)
                    map_addr2[i++] = '.';
                char ch = (funny / p + '0');
                // printf("%d %d %d\n", p, funny, funny / p);
                // printf("%c\n----------\n", ch);

                // map_addr2[i++] = ch;
                funny = funny % p;
                p /= 10;
            }

            memcpy(map_addr2 + i, &ans, sizeof(ans));
            float temp2;
            memcpy(&temp2, map_addr2 + i, sizeof(float));
            printf("numarul float citit este: %f\n", temp2);
            char myresult[100];
            sprintf(myresult, "%f", ans);
            printf("bufferul mru rste: %s", myresult);
            i += sizeof(ans);
            map_addr2[i++] = '\n';
        }
    }

    // if(0 != nr) fprintf(f_dest,"%.2f\n",(float)sum/nr);

    // fclose(f_sursa);
    // fclose(f_dest);

    return 0;
}
