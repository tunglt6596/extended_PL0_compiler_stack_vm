#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Scanner.h"
#include "Parser.h"

TokenType Token;
char initialLetter = ' ';
FILE *f = NULL;
FILE *fo = NULL;
FILE* fo_bin = NULL;

int main(int argc, char** argv)
{
    if(argc != 2) {
        printf("Please enter command and file name...");
        exit(-1);
    }
    char tmp[100];
    strcpy(tmp, argv[1]);
    f = fopen(argv[1], "rt");
    strcpy(strstr(tmp, ".pl0"), ".asm");
    fo = fopen(tmp, "wt");
    strcpy(strstr(tmp, ".asm"), ".bin");
    fo_bin = fopen(tmp, "wb");
    if(f == NULL)
    {
        printf("Cannot open file!!!\n");
        exit(-1);
    }
    Token = getToken();
    compileProgram();
    fclose(f);
    fclose(fo);
    fclose(fo_bin);

    printf("\nResult: \n\n");
    fo_bin = fopen(tmp, "rb");
    pre_processing(fo_bin);
    interpreter();
    return 0;
}
