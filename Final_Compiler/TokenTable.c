#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TokenTable.h"
#include "Scanner.h"

Node *TokenTable = NULL;
extern int pos;
int Tx = 0;
int level = 0, offset = -1;
extern int count_cmds;

void enter(char *Id, Object Type)
{
    Node *new_node = (Node*)calloc(1, sizeof(Node));
    new_node->Id = (char*)calloc(pos + 1, sizeof(char));
    strcpy(new_node->Id, Id);
    new_node->Type = Type;
    new_node->Num_Elems = 0;
    if(Type == PROCEDURE) {
        new_node->lv = level - 1;
        new_node->offset = count_cmds + 1;
    }
    else {
        new_node->lv = level;
        new_node->offset = offset;
    }
    new_node->Next = TokenTable;
    TokenTable = new_node;
    Tx++;
}

int location(char *Id)
{
    Node* tmp = TokenTable;
    int p = Tx;
    while(tmp != NULL && p > 0)
    {
        if(strcmp(Id, tmp->Id) == 0) return p;
        tmp = tmp->Next;
        p--;
    }
    return p;
}

int checkIdent(char *Id, int Ty)
{
    int i = Tx;
    Node *tmp = TokenTable;
    while(tmp != NULL && i > Ty)
    {
        if(strcmp(Id, tmp->Id) == 0) return 1;
        tmp = tmp->Next;
        i--;
    }
    return 0;
}

int getType(int i)
{
    Node *tmp = TokenTable;
    int j = Tx;
    while(j > i)
    {
        tmp = tmp->Next;
        j--;
    }
    return tmp->Type;
}

int getNum_Elems(int i)
{
    Node *tmp = TokenTable;
    int j = Tx;
    while(j > i)
    {
        tmp = tmp->Next;
        j--;
    }
    return tmp->Num_Elems;
}

Node *getNode(int i)
{
    Node *tmp = TokenTable;
    int j = Tx;
    while(j > i)
    {
        tmp = tmp->Next;
        j--;
    }
    return tmp;
}
