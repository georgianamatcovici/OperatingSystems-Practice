#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
void list(const char *name, const char *to_find)
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
char path[PATH_MAX]={0};
strcat(path, name);
strcat(path, "/");
strcat(path, file->d_name);
if(file->d_type==DT_DIR && (strcmp(file->d_name, ".")!=0 && strcmp(file->d_name, "..")!=0))
{


list(path, to_find);

}
else
if(file!=NULL)

{
if(strcmp(file->d_name, to_find)==0)
printf("%s\n", path);
}
}
closedir(dir);

}

int main(int argc, char *argv[])
{
if(argc==1)
{
printf("Error!");
exit(1);
}
struct passwd  *p;
p=getpwuid(getuid());
char *home;
home=p->pw_dir;
list(home, argv[1]);
return 0;
}
