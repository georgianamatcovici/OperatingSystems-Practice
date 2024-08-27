#include <stdio.h>
#define BUF_SIZE 4096

int main(int argc, char *argv[]){
FILE *input_file, *output_file;
ssize_t bytes_in, bytes_out;
char buffer[BUF_SIZE];
if(argc!=3){
printf("Usage: cp file-src file-dest\n");

}

input_file=fopen(argv[1], "rb");
if (input_file==NULL) {
perror(argv[1]); return 2;
}
output_file=fopen(argv[2], "wb");
if(output_file==NULL){
perror(argv[2]); return 3;
}

 while ((bytes_in = fread(buffer, 1, BUF_SIZE, input_file)) > 0) {
        bytes_out = fwrite(buffer, 1, bytes_in, output_file);
        if (bytes_out != bytes_in) {
            fprintf(stderr,"Fatal write error!\n");  return 4;
        }
    }
    fclose(input_file);
    fclose(output_file);
    return 0;
}
