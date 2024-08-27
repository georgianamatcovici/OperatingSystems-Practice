#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char *argv[])
{

DIR *dir;
dir=opendir("/home/georgiana");
struct dirent *element;
while((element=readdir(dir))!=NULL)
{
printf("%s\n", element->d_name);
}

return 0;
}
