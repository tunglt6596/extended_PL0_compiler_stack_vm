#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

FILE* fo = NULL;

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Please enter command and file name...");
        exit(-1);
    }
    FILE* fi = fopen(argv[1], "rb");
    if(fi == NULL)
    {
        printf("File not found\n");
        return -1;
    }
    pre_processing(fi);
    fclose(fi);
  
    char tmp[100];
    strcpy(tmp, argv[1]);
    strcpy(strstr(tmp, ".bin"), ".asm");
    fo = fopen(tmp, "wt");
    if(fo == NULL)
    {
        printf("File cannot be opened\n");
        return -1;
    }

    interpreter();
    
    fclose(fo);
    return 0;
}
