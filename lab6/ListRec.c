#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void list(const char *name)
{
DIR *dir;
dir=opendir(name);
if(dir==NULL)
{
return;
}
struct dirent *file;
while((file=readdir(dir))!=NULL)
{
//printf("%s\n", element->d_name);
if(file->d_type==DT_DIR)
{
printf("\nAcesta este directorul %s, afisare fisiere: \n", file->d_name);
char path[100]={0};
strcat(path, name);
strcat(path, "/");
strcat(path, file->d_name);
list(path);
printf("\n");

}
else
printf("%s\n", file->d_name);


}

}

int main(int argc, char *argv[])
{
list("/home/georgiana");



return 0;
}
