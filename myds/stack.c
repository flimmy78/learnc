#include <stdio.h>
#include "fatal.h"
#include "stack.h"

int CreateStack(Stack S)
{
    if( (S=malloc(sizeof(struct Node))) == NULL)
    {
            FatalError("Out of memory!");
    }

    S->Next = NULL;
    S->Element = '0';
}

Make
