#include <stdio.h>
#include "fatal.h"
#include "stack.h"

struct Node
{
    ElementType Element;
    PtrToNode   Next;
};

Stack CreateStack(void)
{
    Stack S;
    if( (S=malloc(sizeof(struct Node))) == NULL)
    {
	    FatalError("Out of memory!");
    }
    S->Next = NULL;
    //MakeEmpty(S);
	S->Element='\0';
    return S;
}

int IsEmpty(Stack S)
{
    return S->Next == NULL;
}

void Push(ElementType X, Stack S)
{
   PtrToNode tmp;
   tmp = malloc(sizeof(struct Node));
   if(tmp == NULL)
       FatalError("out of memory");
   else
   {
        tmp->Element = X;
        tmp->Next    = S->Next;
        S->Next      = tmp;
   }
}

ElementType Top(Stack S)
{
    if(!IsEmpty(S))
        return S->Next->Element;
    FatalError("Empty stack");
    return '\0';
}

ElementType Pop(Stack S)
{
    PtrToNode tmp;
    ElementType X;
    if(IsEmpty(S))
        return '\0';

    tmp = S->Next;
    S->Next = S->Next->Next;
    X = tmp->Element;
    free(tmp);
    return X;
}

void MakeEmpty(Stack S)
{
    if(IsEmpty(S))
        FatalError("Empty stack");

    while(!IsEmpty(S))
        Pop(S);
}

void Dispose( Stack S )
{
    MakeEmpty( S );
    free( S );
}

int main()
{
    ElementType c;
    Stack S;

    S = CreateStack();
    while( (c=getchar()) != 'D')
        Push(c, S);

    while(!IsEmpty(S))
    {
        c = Pop(S);
        printf("%c", c);
    }
        printf("\n");
	DisposeStack( Stack S );
    return 0;
}
