#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
FILE *fis;
if(argc<3)
{
printf("Eroare");
exit(1);

}

if(access(argv[1], R_OK)==-1)
{
printf("EROARE");
exit(2);
}

fis=fopen(argv[1], "r");



return 0;
}
