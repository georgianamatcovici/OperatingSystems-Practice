#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main (int argc, char *argv[])
{int fb=0, fs=0;
int i;
for(i=1; i<argc; i++)
{if(strcmp(agrv[i], "-b")==0)
fb=1;
if(strcmp(argv[i], "-s")==0)
fs=1;
}
 




return 0;
}

