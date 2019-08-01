#ifndef SCANNER_H
#define SCANNER_H

#define MAX_IDENT_LEN 10

typedef enum{
    NONE = 0, IDENT, NUMBER,
    BEGIN, CALL, CONST, DO, ELSE, END, FOR, IF, ODD,
    PROCEDURE, PROGRAM, THEN, TO, VAR, WHILE,
    PLUS, MINUS, TIMES, SLASH, EQU, NEQ, LSS, LEQ,
    GRT, GEQ, LPARENT, RPARENT, LBRACK, RBRACK,
    PERIOD, COMMA, SEMICOLON, ASSIGN, PERCENT, ARRAY, VAR_PARA
} TokenType;

extern TokenType Token;
extern int Num;
extern char Id[MAX_IDENT_LEN + 1];
extern int pos;
extern char initialLetter;

char getCh();
TokenType getToken();
int is_key_word();
#endif // SCANNER_H_INCLUDED
