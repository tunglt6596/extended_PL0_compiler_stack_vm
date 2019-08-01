#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "Scanner.h"

char *tokenTypes[] = {"NONE", "IDENT", "NUMBER", "BEGIN","CALL", "CONST", "DO",
    "ELSE", "END", "FOR", "IF", "ODD", "PROCEDURE", "PROGRAM", "THEN", "TO",
    "VAR", "WHILE", "PLUS", "MINUS", "TIMES", "SLASH", "EQU", "NEQ", "LSS",
    "LEQ", "GTR", "GEQ", "LPARENT", "RPARENT", "LBRACK", "RBRACK", "PERIOD",
    "COMMA", "SEMICOLON", "ASSIGN", "PERCENT"};

char *tokenSymbols[] =  {" ", "IDENT", "NUMBER", "BEGIN", "CALL", "CONST",
                        "DO", "ELSE", "END", "FOR", "IF", "ODD", "PROCEDURE",
                        "PROGRAM", "THEN", "TO", "VAR", "WHILE", "+", "-",
                        "*", "/", "=", "<>", "<", "<=", ">", ">=", "(", ")"
                        "[", "]", ".", ",", ";", ":=", "%"};

typedef struct TokenName TokenName;

int Num;
char Id[MAX_IDENT_LEN + 1];
int line = 1, column = 1;
int o_line, o_column;
int pos = 0;

extern FILE *f;

int is_key_word()
{
    char tmp[MAX_IDENT_LEN + 1];
    for(int i = 0; i <= strlen(Id); i++)
        tmp[i] = toupper(Id[i]);
    for(int i = 3; i <= 17; i++)
        if(strcmp(tmp, tokenTypes[i]) == 0) return i;
    return 0;
}

void save_to_Id(char c)
{
    if(pos < MAX_IDENT_LEN)
    {
        Id[pos++] = c;
        Id[pos] = '\0';
    }
}

char getCh()
{
    char c = fgetc(f);
    if(c == '\n') {
        line++;
        column = 0;
    }
    column++;
    printf("%c", c);
    return (initialLetter = c);
}

TokenType getToken()
{
    pos = 0;
    char c = initialLetter ;
    TokenType res;
    if(isspace(c)) {
        Token = NONE;
        while( !feof(f) && isspace(c) ) c = getCh();
    }
    o_column = column;
    o_line = line;
    if(isalpha(c)) {
        save_to_Id(c);
        while( !feof(f) && (c = getCh()) ) {
            if (isalpha(c) || isdigit(c)) save_to_Id(c);
            else break;
        }
        TokenType res = is_key_word();
        return (res ? res : IDENT);
    }
    else if(isdigit(c)) {
        save_to_Id(c);
        while( !feof(f) && (c = getCh()) ) {
            if (isdigit(c)){
                save_to_Id(c);
                if(pos > 6) {
                    printf("\n\nerror: appear a too big number!!!\n\n");
                    exit(-1);
                }
            }
            else break;
        }
        Num = 0;
        for(int i = 0; i < pos; i++)
            Num = Num * 10 + Id[i] - '0';
        return NUMBER;
    }
    else if(c == '+') res = PLUS;
    else if(c == '-') res = MINUS;
    else if(c == '*') res = TIMES;
    else if(c == '/') res = SLASH;
    else if(c == '%') res = PERCENT;
    else if(c == '=') res = EQU;
    else if(c == '<') {
        if( (c = getCh()) == '>' ) res = NEQ;
        else if(c == '=') res = LEQ;
        else return LSS;
    }else if(c == '>'){
        if(getCh() == '=') res = GEQ;
        else return GRT;
    }else if(c == '(') {
        if(!feof(f) && getCh() == '*') {
            while( !feof(f) && (c = getCh()) )
                if(c == '*' && !feof(f) && getCh() == ')') break;
            c = getCh();
            return getToken();
        } else return LPARENT;
    }
    else if(c == ')') res = RPARENT;
    else if(c == '[') res = LBRACK;
    else if(c == ']') res = RBRACK;
    else if(c == '.') res = PERIOD;
    else if(c == ',') res = COMMA;
    else if(c == ';') res = SEMICOLON;
    else if(c == ':') {
        if(getCh() == '=') res = ASSIGN;
        else return NONE;
    }
    else res = NONE;
    c = getCh();
    return res;
}


