#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#define MAX 200
#define eroare(msg, cod) \
    {                    \
        perror(msg);     \
        exit(cod);       \
    }
int fd_pipe[2];
int fd_fifo;
void citire_date()
{
    int cod;
    char ch;
    int c;
    int frecv[255] = {0};
    // printf("Date primite de W1\n");
    //  fflush(stdout);
    do
    {

        cod = read(fd_pipe[0], &ch, 1);
        if (cod == -1)
            eroare("eroare citire pipe", 7);
        if (cod > 0)
        {
            c = (int)ch;
            //  printf("%c ", ch);
            //  fflush(stdout);
            frecv[c]++;
        }
    } while (cod > 0);
    // printf("Termin citirea din pipe\n");
    // fflush(stdout);
    for (int i = 0; i <= 255; i++)
        if (frecv[i] != 0)
        {
            char ch = (char)i;
            int ap = frecv[i];
            // printf("SCRIU: %c %d  ", ch, frecv[i]);
            int cod_w = write(fd_fifo, &ch, sizeof(char));
            if (cod_w == -1 || cod_w != 1)
                eroare("eroare write fifo", 7);
            cod_w = write(fd_fifo, &frecv[i], sizeof(int));
            if (cod_w == -1 || cod_w != 4)
                eroare("eroare write fifo", 7);
        }
    close(fd_fifo);
}

void itoa_de_mana(int nr, char fd[10])
{
    int i = 0;
    while (nr)
    {
        fd[i] = (char)((nr % 10) + '0');
        i++;
        nr /= 10;
    }
    fd[i] = '\0';
    for (int i = 0, j = strlen(fd) - 1; i <= j; i++, j--)
    {
        char aux = fd[i];
        fd[i] = fd[j];
        fd[j] = aux;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        eroare("dati fisierul", 2);
    if (pipe(fd_pipe) == -1)
        eroare("eroare pipe", 4);

    int pid = fork();
    if (pid == 0)
    { // fiu
        char fd[10];
        itoa_de_mana(fd_pipe[1], fd);
        // printf("Arg trimise: %s %s %d", argv[1], fd, fd_pipe[1]);
        // fflush(stdout);
        execlp("/home/georgiana/test_practic2/var4/app/supervisor", "supervisor", argv[1], fd, NULL);
        eroare("eroare exec", 2);
    }
    else
    {
        // parinte
        if (-1 == mkfifo("/home/georgiana/test_practic2/var4/app/components/fifo", 0777))
        {
            if (errno != EEXIST)
            {
                eroare("eroare create fifo", 7);
            }
        }
        fd_fifo = open("/home/georgiana/test_practic2/var4/app/components/fifo", O_WRONLY);
        if (fd_fifo == -1)
            eroare("eroare open fifo", 7);
        close(fd_pipe[1]);
        citire_date();
        unlink("/home/georgiana/test_practic2/var4/app/components/fifo");
    }
    return 0;
}