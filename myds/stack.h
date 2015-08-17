#ifndef STACK_H
#define STACK_H
typedef unsigned char ElementType;
struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode Stack;

int StackIsEmpty(Stack S);
Stack CreateStack(void);
void DisposeStack(Stack S);
void MakeEmptyStack(Stack S);
void PushStack(ElementType X, Stack S);
ElementType TopStack(Stack S);
ElementType PopStack(Stack S);
int teststack();
ElementType piarBracket(ElementType c);
void ChkBracket(void);
#endif
