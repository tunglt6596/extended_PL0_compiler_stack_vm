#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Parser.h"
#include "Scanner.h"
#include "TokenTable.h"

char *Ops[] = {"LA", "LV", "LC", "LI", "INT", "DCT", "J", "FJ", "HLT", "ST", "CALL",
               "EP", "EF", "RC", "RI", "WRC", "WRI", "WLN", "ADD", "SUB", "MUL", "DIV",
               "MOD", "NEG", "CV", "EQ", "NE", "GT", "LT", "GE", "LE", "BP"};

extern TokenType Token;
extern int o_line, o_column;
extern int Num;
extern char Id[MAX_IDENT_LEN + 1];
extern Node *TokenTable;
extern int Tx;
extern FILE *fo;
extern FILE *fo_bin;
extern int level, offset;

int count_cmds = 0;
char data[12];

void error(int code) {
    printf("\n\n");
    o_column--;
    if(o_column <= 0) o_column = 1;
    switch(code) {
    case 1:
        printf("error at line %d, column %d: \".\" is expected...", o_line, o_column);
        break;
    case 2:
        printf("error at line %d, column %d: \";\" is expected...", o_line, o_column);
        break;
    case 3:
        printf("error at line %d, column %d: program name is expected...", o_line, o_column);
        break;
    case 4:
        printf("error at line %d, column %d: program keyword is expected...", o_line, o_column);
        break;
    case 5:
        printf("error at line %d, column %d: ] keyword is expected...", o_line, o_column);
        break;
    case 6:
        printf("error at line %d, column %d: ) is expected in expression...", o_line, o_column);
        break;
    case 7:
        printf("error at line %d, column %d: need a value or expression here...", o_line, o_column);
        break;
    case 8:
        printf("error at line %d, column %d: a relational operator is expected...", o_line, o_column);
        break;
    case 9:
        printf("error at line %d, column %d: assign operator is expected...", o_line, o_column);
        break;
    case 10:
        printf("error at line %d, column %d: a name is expected...", o_line, o_column);
        break;
    case 11:
        printf("error at line %d, column %d: end keyword is expected...", o_line, o_column);
        break;
    case 12:
        printf("error at line %d, column %d: then keyword is expected...", o_line, o_column);
        break;
    case 14:
        printf("error at line %d, column %d: do keyword is expected...", o_line, o_column);
        break;
    case 15:
        printf("error at line %d, column %d: a variable is expected...", o_line, o_column);
        break;
    case 16:
        printf("error at line %d, column %d: to keyword is expected...", o_line, o_column);
        break;
    case 17:
        printf("error at line %d, column %d: equal sign is expected...", o_line, o_column);
        break;
    case 18:
        printf("error at line %d, column %d: a value is expected...", o_line, o_column);
        break;
    case 19:
        printf("error at line %d, column %d: ; is expected...", o_line, o_column);
        break;
    case 20:
        printf("error at line %d, column %d: begin keyword is needed...", o_line, o_column);
        break;
    case 21:
        printf("error at line %d, column %d: a parameter is expected...", o_line, o_column);
        break;
    case 22:
        printf("error at line %d, column %d: variable name %s has been duplicated...", o_line, o_column, Id);
        break;
    case 23:
        printf("error at line %d, column %d: variable %s has not been declared...", o_line, o_column, Id);
        break;
    case 24:
        printf("error at line %d, column %d: the left side must be a variable...", o_line, o_column);
        break;
    case 25:
        printf("error at line %d, column %d: constant %s has not been declared...", o_line, o_column, Id);
        break;
    case 26:
        printf("error at line %d, column %d: name %s has been duplicated...", o_line, o_column, Id);
        break;
    case 27:
        printf("error at line %d, column %d: name %s has not been declare...", o_line, o_column, Id);
        break;
    case 28:
        printf("error at line %d, column %d: %s is not an valid value...", o_line, o_column, Id);
        break;
    case 29:
        printf("error at line %d, column %d: procedure name %s has been duplicated...", o_line, o_column, Id);
        break;
    case 30:
        printf("error at line %d, column %d: a parameter has been duplicated...", o_line, o_column);
        break;
    case 31:
        printf("error at line %d, column %d: %s is not a variable...", o_line, o_column, Id);
        break;
    case 32:
        printf("error at line %d, column %d: %s is not an array...", o_line, o_column, Id);
        break;
    case 33:
        printf("error at line %d, column %d: %s is not a single variable...", o_line, o_column, Id);
        break;
    case 34:
        printf("error at line %d, column %d: %s is not an variable or a constant...", o_line, o_column, Id);
        break;
    case 35:
        printf("error at line %d, column %d: has a lack of parameters for calling procedure...", o_line, o_column, Id);
        break;
    case 36:
        printf("error at line %d, column %d: there are too many parameters than expected...", o_line, o_column, Id);
        break;
    case 37:
        printf("error at line %d, column %d: declare constants is invalid here...", o_line, o_column, Id);
        break;
    case 38:
        printf("error at line %d, column %d: declare variables is invalid here...", o_line, o_column, Id);
        break;
    case 39:
        printf("error at line %d, column %d: procedure %s has not declared...", o_line, o_column, Id);
        break;
    case 40:
        printf("error at line %d, column %d: procedure needs only one parameter...", o_line, o_column, Id);
        break;
    case 41:
        printf("error at line %d, column %d: need a variable here...", o_line, o_column, Id);
        break;
    }
    printf("\n\n");
    exit(-1);
}

void compileFactor() {
    int p;
    int t;
    if(Token == IDENT) {
        p = location(Id);
        if(p <= 0) error(27);
        if( (t=getType(p)) != VAR && t != CONST && t != ARRAY && t != VAR_PARA) error(28);
        Node* tmp = getNode(p);
        if(t == ARRAY) genCode(OP_LA, 2, level-tmp->lv, tmp->offset);
        else genCode(OP_LV, 2, level-tmp->lv, tmp->offset);
        if(t == VAR_PARA) genCode(OP_LI, 0, 0, 0);
        Token = getToken();
        if(Token == LBRACK) {
            if(t != ARRAY) error(32);
            genCode(OP_LC, 1, 0, 1);
            Token = getToken();
            compileExpression();
            genCode(OP_MUL, 0, 0, 0);
            genCode(OP_ADD, 0, 0, 0);
            genCode(OP_LI, 0, 0, 0);
            if(Token == RBRACK) Token = getToken();
            else error(5);
        }else if(t == ARRAY) error(34);
    }
    else if(Token == NUMBER){
        genCode(OP_LC, 1, 0, Num);
        Token = getToken();
    }
    else if(Token == LPARENT) {
        Token = getToken();
        compileExpression();
        if(Token == RPARENT) Token = getToken();
        else error(6);
    }
    else error(7);
}

void compileTerm() {
    TokenType Op;
    compileFactor();
    while(Token == TIMES || Token == SLASH || Token == PERCENT) {
        Op = Token;
        Token = getToken();
        compileFactor();
        if(Op == TIMES) genCode(OP_MUL, 0, 0, 0);
        else if(Op == SLASH) genCode(OP_DIV, 0, 0, 0);
        else if(Op == PERCENT) genCode(OP_MOD, 0, 0, 0);
    }
}

void compileExpression() {
    TokenType Op = NONE;
    if(Token == PLUS || Token == MINUS)
    {
        Op = Token;
        Token = getToken();
    }
    compileTerm();
    if(Op == MINUS) genCode(OP_NEG, 0, 0, 0);
    while(Token == PLUS || Token == MINUS) {
        Op = Token;
        Token = getToken();
        compileTerm();
        if(Op == PLUS) genCode(OP_ADD, 0, 0, 0);
        else if(Op == MINUS) genCode(OP_SUB, 0, 0, 0);
    }
}

void compileCondition(int *pos_asm, int *pos_bin) {
    TokenType Op;
    if(Token == ODD) {
        Token = getToken();
        compileExpression();
    }else{
        compileExpression();
        if(Token == EQU || Token == NEQ || Token == LSS || Token == LEQ
           || Token == GRT || Token == GEQ) {
            Op = Token;
            Token = getToken();
            compileExpression();
            if(Op == EQU) genCode(OP_EQ, 0, 0, 0);
            else if(Op == NEQ) genCode(OP_NE, 0, 0, 0);
            else if(Op == LSS) genCode(OP_LT, 0, 0, 0);
            else if(Op == LEQ) genCode(OP_LE, 0, 0, 0);
            else if(Op == GRT) genCode(OP_GT, 0, 0, 0);
            else if(Op == GEQ) genCode(OP_GE, 0, 0, 0);
        } else error(8);
        *pos_asm = ftell(fo);
        *pos_bin = ftell(fo_bin);
        fprintf(fo, "               ");
        fwrite(data, 1, sizeof(data), fo_bin);
        count_cmds++;
    }
}

void compileProgram() {
    if(Token == PROGRAM) {
        genCode(OP_J, 1, 0, 1);
        Token = getToken();
        if(Token == IDENT) {
            Token = getToken();
            if(Token == SEMICOLON) {
                Token = getToken();
                compileBlock(0);
                if(Token == PERIOD)
                {
                    printf("\n\nThe program is syntactically correct...\n\n");
                    genCode(OP_HL, 0, 0, 0);
                }else error(1);
            }else error(2);
        }else error(3);
    }else error(4);
}

void compileLvalue() {
    int p, t;
    if(Token == IDENT) {
        p = location(Id);
        if(p <= 0) error(23);
        if( (t = getType(p)) != VAR && t != ARRAY && t != VAR_PARA) error(24);
        Node* tmp = getNode(p);
        genCode(OP_LA, 2, level-tmp->lv, tmp->offset);
        if(t == VAR_PARA) genCode(OP_LI, 0, 0, 0);
        Token = getToken();
        if(Token == LBRACK) {
            if(t != ARRAY) error(32);
            genCode(OP_LC, 1, 0, 1);
            Token = getToken();
            compileExpression();
            genCode(OP_MUL, 0, 0, 0);
            genCode(OP_ADD, 0, 0, 0);
            if(Token == RBRACK) Token = getToken();
            else error(5);
        }else if(t != VAR && t != VAR_PARA) error(33);
    }else error(41);
}

void compileStatement() {
    int p, t;
    if(Token == IDENT) {
        p = location(Id);
        if(p <= 0) error(23);
        if( (t = getType(p)) != VAR && t != ARRAY && t != VAR_PARA) error(24);
        Node* tmp = getNode(p);
        if(t == VAR_PARA) genCode(OP_LV, 2, level-tmp->lv, tmp->offset);
        else genCode(OP_LA, 2, level-tmp->lv, tmp->offset);
        Token = getToken();
        if(Token == LBRACK) {
            if(t != ARRAY) error(32);
            genCode(OP_LC, 1, 0, 1);
            Token = getToken();
            compileExpression();
            genCode(OP_MUL, 0, 0, 0);
            genCode(OP_ADD, 0, 0, 0);
            if(Token == RBRACK) Token = getToken();
            else error(5);
        }else if(t != VAR && t != VAR_PARA) error(33);
        if(Token == ASSIGN) {
            Token = getToken();
            compileExpression(Tx);
            genCode(OP_ST, 0, 0, 0);
        }else error(9);
    }else if(Token == CALL) {
        Token = getToken();
        if(Token == IDENT) {
            char tmp[MAX_IDENT_LEN + 1];
            for(int i = 0; i <= strlen(Id); i++)
                tmp[i] = tolower(Id[i]);
            if(strcmp(tmp, "read") == 0 || strcmp(tmp, "readln") == 0 ||
               strcmp(tmp, "write") == 0 || strcmp(tmp, "writeln") == 0)
            {
                Token = getToken();
                if(Token == LPARENT) {
                    Token = getToken();
                    if(strcmp(tmp, "write") == 0 || strcmp(tmp, "writeln") == 0)
                        compileExpression();
                    else compileLvalue();
                }
                if(strcmp(tmp, "write") == 0)
                    genCode(OP_WRI, 0, 0, 0);
                else if(strcmp(tmp, "read") == 0)
                    genCode(OP_RI, 0, 0, 0);
                else if(strcmp(tmp,"writeln") == 0) {
                    genCode(OP_WRI, 0, 0, 0);
                    genCode(OP_WLN, 0, 0, 0);
                }
                else if(strcmp(tmp, "readln") == 0)
                {
                    genCode(OP_RI, 0, 0, 0);
                    genCode(OP_WLN, 0, 0, 0);
                }
                if(Token == RPARENT) Token = getToken();
                else error(6);
            }
            else
            {
                int cnt_paras = 0;
                genCode(OP_INT, 1, 0, 4);
                p = location(Id);
                if(p <= 0) error(39);
                if(getType(p) != PROCEDURE) error(28);
                Token = getToken();
                Node* tmp = getNode(p);
                if(Token == LPARENT) {
                    Token = getToken();
                    cnt_paras++;
                    if(tmp->type_paras[cnt_paras]) compileLvalue();
                    else compileExpression();
                    while(Token == COMMA) {
                        Token = getToken();
                        cnt_paras++;
                        if(tmp->type_paras[cnt_paras]) compileLvalue();
                        else compileExpression();
                    }
                    if(Token == RPARENT) {
                        int num_paras = getNum_Elems(p);
                        if(num_paras > cnt_paras) error(35);
                        else if(num_paras < cnt_paras) error(36);
                        Token = getToken();
                    }else error(6);
                }
                genCode(OP_DCT, 1, 0, 4 + cnt_paras);
                genCode(OP_CALL, 2, level-tmp->lv, tmp->offset + level);
            }
        }else error(10);
    }else if(Token == BEGIN) {
        Token = getToken();
        compileStatement();
        if(Token != SEMICOLON && Token != END) {
            if(pos == 0) error(11);
            o_column -= pos;
            if(o_column <= 0) {
                o_column = 1;
                o_line--;
            }
            error(19);
        }
        while(Token == SEMICOLON) {
            Token = getToken();
            compileStatement();
            if(Token != SEMICOLON && Token != END) {
                if(pos == 0) error(11);
                o_column -= pos;
                if(o_column <= 0) {
                    o_column = 1;
                    o_line--;
                }
                error(19);
            }
        }
        if(Token == END) Token = getToken();
    }else if(Token == IF) {
        int pos_asm, pos_bin;
        Token = getToken();
        compileCondition(&pos_asm, &pos_bin);
        if(Token == THEN) {
            Token = getToken();
            compileStatement();
            fseek(fo, pos_asm, SEEK_SET);
            fseek(fo_bin, pos_bin, SEEK_SET);
            genCode(OP_FJ, 1, 0, count_cmds+1);
            count_cmds--;
            fseek(fo, 0, SEEK_END);
            fseek(fo_bin, 0, SEEK_END);
            if(Token == ELSE) {
                int pos_asm_j = ftell(fo);
                int pos_bin_j = ftell(fo_bin);
                fprintf(fo, "               ");
                fwrite(data, 1, sizeof(data), fo_bin);
                count_cmds++;
                fseek(fo, pos_asm, SEEK_SET);
                fseek(fo_bin, pos_bin, SEEK_SET);
                genCode(OP_FJ, 1, 0, count_cmds + level);
                count_cmds--;
                fseek(fo, 0, SEEK_END);
                fseek(fo_bin, 0, SEEK_END);
                Token = getToken();
                compileStatement();
                fseek(fo, pos_asm_j, SEEK_SET);
                fseek(fo_bin, pos_bin_j, SEEK_SET);
                genCode(OP_J, 1, 0, count_cmds + level);
                count_cmds--;
                fseek(fo, 0, SEEK_END);
                fseek(fo_bin, 0, SEEK_END);
            }
        }else error(12);
    }else if(Token == WHILE) {
        int count_cmds_tmp = count_cmds;
        int pos_asm, pos_bin;
        Token = getToken();
        compileCondition(&pos_asm, &pos_bin);
        if(Token == DO) {
            Token = getToken();
            compileStatement();
            genCode(OP_J, 1, 0, count_cmds_tmp + level);
            fseek(fo, pos_asm, SEEK_SET);
            fseek(fo_bin, pos_bin, SEEK_SET);
            genCode(OP_FJ, 1, 0, count_cmds + level);
            count_cmds--;
            fseek(fo, 0, SEEK_END);
            fseek(fo_bin, 0, SEEK_END);
        }else error(14);
    }else if(Token == FOR) {
        int t;
        int pos_j1, pos_j2_asm, pos_j2_bin;
        Token = getToken();
        if(Token == IDENT) {
            p = location(Id);
            if(p <= 0) error(27);
            if( (t=getType(p)) != VAR && t != VAR_PARA) error(31);
            compileLvalue();
            genCode(OP_CV, 0, 0, 0);
            if(Token == ASSIGN) {
                Token = getToken();
                compileExpression();
                genCode(OP_ST, 0, 0, 0);
                pos_j1 = count_cmds + level;
                if(Token == TO) {
                    genCode(OP_CV, 0, 0, 0);
                    genCode(OP_LI, 0, 0, 0);
                    Token = getToken();
                    compileExpression();
                    genCode(OP_LE, 0, 0, 0);
                    pos_j2_asm = ftell(fo);
                    pos_j2_bin = ftell(fo_bin);
                    fprintf(fo, "               ");
                    fwrite(data, 1, sizeof(data), fo_bin);
                    count_cmds++;
                    if(Token == DO) {
                        Token = getToken();
                        compileStatement();
                        genCode(OP_CV, 0, 0, 0);
                        genCode(OP_CV, 0, 0, 0);
                        genCode(OP_LI, 0, 0, 0);
                        genCode(OP_LC, 1, 0, 1);
                        genCode(OP_ADD, 0, 0, 0);
                        genCode(OP_ST, 0, 0, 0);
                        genCode(OP_J, 1, 0, pos_j1);
                        fseek(fo, pos_j2_asm, SEEK_SET);
                        fseek(fo_bin, pos_j2_bin, SEEK_SET);
                        genCode(OP_FJ, 1, 0, count_cmds + level);
                        count_cmds--;
                        fseek(fo, 0, SEEK_END);
                        fseek(fo_bin, 0, SEEK_END);
                        genCode(OP_DCT, 1, 0, 1);
                    }else error(14);
                }else error(16);
            }else error(9);
        }else error(15);
    }
}

void compileDeclareConst(int Ty) {
    if(Token == IDENT) {
        if(checkIdent(Id, Ty) == 0){
            offset++;
            enter(Id, CONST);
        }
        else error(26);
        Token = getToken();
        if(Token == EQU) {
            Token = getToken();
            if(Token == MINUS) {
                Token = getToken();
                if(Token == NUMBER) {
                    Num = -Num;
                    Token = getToken();
                }
                else error(18);
            } else if (Token == NUMBER) Token = getToken();
            else error(18);
            genCode(OP_LC, 1, 0, Num);
        }else error(17);
    }
    else error(10);
}

void compileDeclareVar(int Ty) {
    if(Token == IDENT) {
        if(checkIdent(Id, Ty) == 0){
            offset++;
            enter(Id, VAR);
            genCode(OP_LC, 1, 0, 0);
        }
        else error(22);
        Token = getToken();
        if(Token == LBRACK) {
            TokenTable->Type = ARRAY;
            Token = getToken();
            if(Token == NUMBER) {
                TokenTable->Num_Elems = Num;
                for(int i = 0; i < Num - 1; i++)
                    genCode(OP_LC, 1, 0, 0);
                offset += Num - 1;
                Token = getToken();
                if(Token == RBRACK) Token = getToken();
                else error(5);
            }else error(18);
        }
    }else error(10);
}

void compileBlock(int Ty) {
    int Tx_new;
    long pos_file_asm, pos_file_bin;
    compile_const:
    if(Token == CONST) {
        Token = getToken();
        compileDeclareConst(Ty);
        while(Token == COMMA) {
            Token = getToken();
            compileDeclareConst(Ty);
        }
        if(Token == SEMICOLON) {
            Token = getToken();
            goto compile_const;
        }else error(19);
    }

    compile_var:
    if(Token == VAR) {
        Token = getToken();
        compileDeclareVar(Ty);
        while(Token == COMMA) {
            Token = getToken();
            compileDeclareVar(Ty);
        }
        if(Token == SEMICOLON) {
            Token = getToken();
            if(Token == CONST) error(37);
            goto compile_var;
        }else error(19);
    }

    if(Token == PROCEDURE) {
        Node *tmp_node;
        pos_file_asm = ftell(fo);
        fprintf(fo, "               ");
        pos_file_bin = ftell(fo_bin);
        fwrite(data, 1, sizeof(data), fo_bin);
        while(Token == PROCEDURE) {
            level++;
            offset = 3;
            Token = getToken();
            if(Token == IDENT) {
                if(checkIdent(Id, Ty) == 0) {
                    enter(Id, PROCEDURE);
                    Tx_new = Tx - 1;
                }else error(29);
                Token = getToken();
                tmp_node = TokenTable;
                if(Token == LPARENT) {
                    Token = getToken();
                    if(Token == VAR) {
                        Token = getToken();
                        if(Token == IDENT) {
                            if(checkIdent(Id, Tx_new) == 0) {
                                tmp_node->Num_Elems++;
                                tmp_node->type_paras[tmp_node->Num_Elems] = 1;
                                offset++;
                                enter(Id, VAR_PARA);
                            }
                            else error(30);
                            Token = getToken();
                        }
                        else error(10);
                    }else if(Token == IDENT) {
                        if(checkIdent(Id, Tx_new) == 0) {
                            tmp_node->Num_Elems++;
                            tmp_node->type_paras[tmp_node->Num_Elems] = 0;
                            offset++;
                            enter(Id, VAR);
                        }
                        else error(30);
                        Token = getToken();
                    }else error(21);
                    while(Token == SEMICOLON) {
                        Token = getToken();
                        if(Token == VAR) {
                            Token = getToken();
                            if(Token == IDENT) {
                                if(checkIdent(Id, Tx_new) == 0) {
                                    tmp_node->Num_Elems++;
                                    tmp_node->type_paras[tmp_node->Num_Elems] = 1;
                                    offset++;
                                    enter(Id, VAR_PARA);
                                }
                                else error(30);
                                Token = getToken();
                            }
                            else error(10);
                        }
                        else if(Token == IDENT) {
                            Token = getToken();
                            if(checkIdent(Id, Tx_new) == 0) {
                                tmp_node->Num_Elems++;
                                tmp_node->type_paras[tmp_node->Num_Elems] = 0;
                                offset++;
                                enter(Id, VAR);
                            }
                            else error(30);
                        }else error(21);
                    }
                    if(Token == RPARENT) Token = getToken();
                    else error(6);
                }
                if(Token == SEMICOLON) {
                    genCode(OP_INT, 1, 0, 4 + tmp_node->Num_Elems);
                    Token = getToken();
                    compileBlock(Tx_new);
                    if(Token == SEMICOLON) Token = getToken();
                    else error(19);
                }else error(19);
            }else error(10);
            genCode(OP_EP, 0, 0, 0);
            level--;
        }
        if(Token == CONST) error(37);
        else if(Token == VAR) error(38);

        fseek(fo, pos_file_asm, SEEK_SET);
        fseek(fo_bin, pos_file_bin, SEEK_SET);
        genCode(OP_J, 1, 0, count_cmds+level+1);
        fseek(fo, 0, SEEK_END);
        fseek(fo_bin, 0, SEEK_END);
    }

    if(Token == BEGIN) {
        Token = getToken();
        compileStatement(Tx);
        if(Token != SEMICOLON && Token != END) {
            if(pos == 0 || Token == PROCEDURE) error(11);
            o_column -= pos;
            if(o_column <= 0) {
                o_column = 1;
                o_line--;
            }
            error(19);
        }
        while(Token == SEMICOLON) {
            Token = getToken();
            compileStatement(Tx);
            if(Token != SEMICOLON && Token != END) {
                if(pos == 0 || Token == PROCEDURE) error(11);
                o_column -= pos;
                if(o_column <= 0) {
                    o_column = 1;
                    o_line--;
                }
                error(19);
            }
        }
        if(Token == END) Token = getToken();
    }else error(20);

    while(Tx > Ty + 1)
    {
        Node* tmp = TokenTable;
        TokenTable = TokenTable->Next;
        free(tmp->Id);
        free(tmp);
        Tx--;
    }
}

void genCode(OpCode Op, int num, int p, int q)
{
    count_cmds++;
    if(num == 0) fprintf(fo, "\n%s", Ops[Op]);
    else if(num == 1) fprintf(fo, "\n%s %d", Ops[Op], q);
    else if(num == 2) fprintf(fo, "\n%s %d, %d", Ops[Op], p, q);
    fwrite(&Op, 1, 4, fo_bin);
    fwrite(&p, 1, 4, fo_bin);
    fwrite(&q, 1, 4, fo_bin);
}
