#include <stdio.h>
#include <string.h>
#include "Interpreter.h"

Instruction Code[MAX_CODE];
int Stack[MAX_DATA];
int Exit = 0;
int pc = 0, t = -1, b = 0;

void pre_processing(FILE *fi)
{
    char read_data[12];
    int i = 0;
    while(!feof(fi) && fread(read_data, sizeof(read_data), 1, fi) > 0)
    {
        strncpy((char*)&(Code[i].Op), read_data, 4);
        strncpy((char*)&(Code[i].p), read_data + 4, 4);
        strncpy((char*)&(Code[i].q), read_data + 8, 4);
        i++;
    }
    fclose(fi);
}

int base(int L)
{
    int c = b;
    while(L > 0)
    {
        c = Stack[c + 3];
        L--;
    }
    return c;
}

void interpreter()
{
    int n;
    do {
        switch(Code[pc].Op) {
        case OP_LA: t++;
            Stack[t] = base(Code[pc].p) + Code[pc].q;
            break;
        case OP_LV: t++;
            Stack[t] = Stack[base(Code[pc].p) + Code[pc].q];
            break;
        case OP_LC:
            t++;
            Stack[t] = Code[pc].q;
            break;
        case OP_LI:
            Stack[t] = Stack[Stack[t]];
            break;
        case OP_INT:
            t += Code[pc].q;
            break;
        case OP_DCT:
            t -= Code[pc].q;
            break;
        case OP_J:
            pc = Code[pc].q - 1;
            break;
        case OP_FJ:
            if(Stack[t] == 0) pc = Code[pc].q - 1;
            t--;
            break;
        case OP_HL:
            Exit = 1;
            break;
        case OP_ST:
            Stack[Stack[t-1]] = Stack[t];
            t -= 2;
            break;
        case OP_CALL:
            Stack[t + 2] = b;
            Stack[t + 3] = pc;
            Stack[t + 4] = base(Code[pc].p);
            b = t + 1;
            pc = Code[pc].q - 1;
            break;
        case OP_EP:
            t = b - 1;
            pc = Stack[b + 2];
            b = Stack[b + 1];
            break;
        case OP_EF:
            t = b;
            pc = Stack[b + 2];
            b = Stack[b + 1];
            break;
        case OP_RC:
            Stack[Stack[t]] = getchar();
            t--;
            break;
        case OP_RI:
            scanf("%d", &n);
            Stack[Stack[t]] = n;
            t--;
            break;
        case OP_WRC:
            putchar(Stack[t]);
            t--;
            break;
        case OP_WRI:
            printf("%d", Stack[t]);
            t--;
            break;
        case OP_WLN:
            printf("\n");
            break;
        case OP_ADD: t--;
            Stack[t] += Stack[t + 1];
            break;
        case OP_SUB: t--;
            Stack[t] -= Stack[t + 1];
            break;
        case OP_MUL: t--;
            Stack[t] *= Stack[t + 1];
            break;
        case OP_DIV: t--;
            Stack[t] /= Stack[t + 1];
            break;
        case OP_MOD: t--;
            Stack[t] %= Stack[t + 1];
            break;
        case OP_NEG:
            Stack[t] = -Stack[t];
            break;
        case OP_CV:
            Stack[t + 1] = Stack[t];
            t++;
            break;
        case OP_EQ: t--;
            if(Stack[t] == Stack[t + 1]) Stack[t] = 1;
            else Stack[t] = 0;
            break;
        case OP_NE: t--;
            if(Stack[t] != Stack[t + 1]) Stack[t] = 1;
            else Stack[t] = 0;
            break;
        case OP_GT: t--;
            if(Stack[t] > Stack[t + 1]) Stack[t] = 1;
            else Stack[t] = 0;
            break;
        case OP_LT: t--;
            if(Stack[t] < Stack[t + 1]) Stack[t] = 1;
            else Stack[t] = 0;
            break;
        case OP_GE: t--;
            if(Stack[t] >= Stack[t + 1]) Stack[t] = 1;
            else Stack[t] = 0;
            break;
        case OP_LE: t--;
            if(Stack[t] <= Stack[t + 1]) Stack[t] = 1;
            else Stack[t] = 0;
            break;
        case OP_BP:
            break;
        }
        pc++;
    }while(Exit == 0);
}
