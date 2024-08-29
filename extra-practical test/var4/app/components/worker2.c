#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#define MAX 200
#define shm_SIZE 4096
#define eroare(mesaj, cod) \
    {                      \
        perror(mesaj);     \
        exit(cod);         \
    }
int fd_fifo;
int *shm_ptr;
void citire_date()
{
    int cod;
    char ch;
    int ap;
    // printf("Date primite W2: \n");
    // fflush(stdout);
    int max_voc = 0, min_cons = 10000000;
    char voc, cons;
    // printf("Descriptor fifo: %d", fd_fifo);
    do
    {
        cod = read(fd_fifo, &ch, sizeof(char));
        if (cod == -1)
            eroare("eroare citire fifo", 8);
        cod = read(fd_fifo, &ap, sizeof(int));
        if (cod == -1)
            eroare("eroare citire fifo", 8);
        if (cod > 0)
        {
            printf("%c %d \n", ch, ap);
            // fflush(stdout);
            if (strchr("aeiou", ch) != NULL)
            {
                if (ap > max_voc)
                {
                    max_voc = ap;
                    voc = ch;
                }
                else if (ap == max_voc && ch < voc)
                    voc = ch;
            }
            else
            {
                if (ap < min_cons)
                {
                    min_cons = ap;
                    cons = ch;
                }
                else if (ap == min_cons && ch < cons)
                    cons = ch;
            }
        }

    } while (cod > 0);
    // printf("trimit: %c %d  %c %d", voc, max_voc, cons, min_cons);
    sem_t *sem = sem_open("sem", O_CREAT, 0644, 0);
    sem_post(sem);
    shm_ptr[0] = (int)voc;
    shm_ptr[1] = max_voc;
    shm_ptr[2] = (int)cons;
    shm_ptr[3] = min_cons;
    sem_unlink("sem");
}
int main(int argc, char *argv[])
{
    if (-1 == mkfifo("/home/georgiana/test_practic2/var4/app/components/fifo", 0777))
    {
        if (errno != EEXIST)
        {
            eroare("eroare create fifo", 7);
        }
    }
    fd_fifo = open("/home/georgiana/test_practic2/var4/app/components/fifo", O_RDONLY);
    if (fd_fifo == -1)
        eroare("eroare open fifo", 7);
    // printf("Fd fifo: %d", fd_fifo);

    int shm_fd = shm_open("shm_obj", O_CREAT | O_RDWR, 0600);
    ftruncate(shm_fd, shm_SIZE);
    shm_ptr = mmap(NULL, shm_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    citire_date();
    shm_unlink("shm_obj");
    unlink("/home/georgiana/test_practic2/var4/app/components/fifo");

    return 0;
}