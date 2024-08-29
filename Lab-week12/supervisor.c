#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#define MAX 200
#define MAX_NO_WORKERS 10

int fd_pipe[MAX][2];
int NW = 0;
void pipe_init(int n)
{
	for (int i = 0; i < n; i++)
		if (pipe(fd_pipe[i]) == -1)
		{
			perror("Eroare la crearea unui pipe");
			exit(2);
		}
}

void worker(int k)
{
	int r_code, nr, sum = 0;
	do
	{
		r_code = read(fd_pipe[k - 1][0], &nr, sizeof(int));
		if (r_code != -1 && r_code != 0)
			sum += nr;
		printf("Citire worker %d : %d\n", k, nr);
	} while (r_code != 0);
	// printf("Citire finalizata %d\n", k);

	write(fd_pipe[NW][1], &sum, sizeof(int));
}

int main()
{

	int nr = 0, k, i, pid;

	printf("Dati numarul de procese workers: ");
	while (1 != scanf("%d", &NW))
	{
		fprintf(stderr, "Trebuie sa introduceti un numar intreg pozitiv! Incercati din nou...\n");
	}
	if ((NW < 1) || (NW > MAX_NO_WORKERS))
	{
		fprintf(stderr, "Date de intrare incorecte.\n");
		exit(1);
	}
	pipe_init(NW + 1);

	for (i = 1; i <= NW; ++i)
	{
		if (-1 == (pid = fork()))
		{
			fprintf(stderr, "[Supervisor] Eroare la apelul fork() pentru procesul worker al %d-lea.\n", i);
			perror("Cauza erorii: ");
			exit(3);
		}

		if (pid == 0)
		{
			for (int j = 0; j < NW; j++)
				close(fd_pipe[j][1]);

			worker(i);
			close(fd_pipe[NW][1]);

			return i;
		}
	}
	close(fd_pipe[NW][1]);
	printf("Introduceti secventa de numere (0 pentru terminare) :\n");
	k = 0;
	do
	{
		while (1 != scanf("%d", &nr))
		{
			fprintf(stderr, "Trebuie sa introduceti un numar intreg nenul! Incercati din nou...\n");
		}
		if (nr != 0)
		{
			write(fd_pipe[k][1], &nr, sizeof(int));
		}
		k = (k + 1) % NW;
	} while (nr != 0);

	for (i = 0; i < NW; i++)
	{
		close(fd_pipe[i][1]);
	}

	for (i = 1; i <= NW; ++i)
	{
		wait(NULL);
	}
	int sum = 0;
	int sum1 = 0;

	int r_code;
	i = 1;
	do
	{
		r_code = read(fd_pipe[NW][0], &sum1, sizeof(int));
		if (r_code == -1)
		{
			exit(7);
		}
		printf("Suma obtinuta de worker-ul %d: %d\n", i, sum1);
		if (r_code != 0)
			sum += sum1;
		i++;
	} while (r_code != 0);

	printf("[Supervisor] Suma calculata a secventei introduse este: %d.\n", sum);

	return 0;
}