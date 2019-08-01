#ifndef PARSER_H
#define PARSER_H
#include "Interpreter.h"
void error(int code);
void compileFactor();
void compileTerm();
void compileExpression();
void compileCondition(int*, int*);
void compileStatement();
void compileDeclareConst(int);
void compileDeclareVar(int);
void compileBlock(int);
void compileProgram();
void compileLvalue();
void genCode(OpCode, int, int, int);
#endif // PARSER_H_INCLUDED
