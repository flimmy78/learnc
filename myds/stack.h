#ifndef STACK_H
#define STACK_H
typedef unsigned char ElementType;
struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode Stack;

int IsEmpty(Stack S);
int CreateStack(Stack S);

struct Node
{
    ElementType Element;
    PtrToNode Next;
};
#endif
