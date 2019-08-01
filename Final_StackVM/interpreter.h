#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#define MAX_CODE 3000
#define MAX_DATA 3000

typedef enum {
    OP_LA = 0,  //Load Address
    OP_LV,  //Load Value
    OP_LC,  //Load Constant
    OP_LI,  //Load Indirect
    OP_INT, //Increment t
    OP_DCT, //Decrement t
    OP_J,   //Jump
    OP_FJ,  //False Jump
    OP_HL,  //Halt
    OP_ST,  //Store
    OP_CALL,//Call
    OP_EP,  //Exit Procedure
    OP_EF,  //Exit Function
    OP_RC,  //Read Char
    OP_RI,  //Read Integer
    OP_WRC, //Write Char
    OP_WRI, //Write Int
    OP_WLN, //WriteLN
    OP_ADD, //Add
    OP_SUB, //Subtract
    OP_MUL, //Multiple
    OP_DIV, //Divide
    OP_NEG, //Negative
    OP_CV,  //Copy Top
    OP_EQ,  //Equal
    OP_NE,  //Not equal
    OP_GT,  //Greater
    OP_LT,  //Less
    OP_GE,  //Greater or equal
    OP_LE,  //Less or equal
    OP_BP   //Break point
} OpCode;

typedef struct{
    OpCode Op;
    int p;
    int q;
} Instruction;

void pre_processing(FILE *fi);
int base(int L);
void interpreter();
void genAssemblyFile();
#endif // INTERPRETER_H_INCLUDED
