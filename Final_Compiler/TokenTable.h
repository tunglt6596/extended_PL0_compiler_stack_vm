#ifndef TOKENTABLE_H_INCLUDED
#define TOKENTABLE_H_INCLUDED

#include "Scanner.h"

typedef int Object;

struct Node
{
    char *Id;
    Object Type;
    int Num_Elems;
    int type_paras[20];
    int lv;
    int offset;
    struct Node *Next;
};

typedef struct Node Node;

void enter(char *Id, Object Type);
int location(char *Id);
int checkIdent(char *Id, int Ty);
int getType(int pos);
int getNum_Elems(int i);
Node *getNode(int i);

#endif // TOKENTABLE_H_INCLUDED
