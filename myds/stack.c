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

int StackIsEmpty(Stack S)
{
    return S->Next == NULL;
}

void PushStack(ElementType X, Stack S)
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

ElementType TopStack(Stack S)
{
    if(!StackIsEmpty(S))
        return S->Next->Element;
    FatalError("Empty stack");
    return '\0';
}

ElementType PopStack(Stack S)
{
    PtrToNode tmp;
    ElementType X;
    if(StackIsEmpty(S))
        return '\0';

    tmp = S->Next;
    S->Next = S->Next->Next;
    X = tmp->Element;
    free(tmp);
    return X;
}

void MakeEmptyStack(Stack S)
{
    if(StackIsEmpty(S))
        printf("Stack is Empty \n");

    while(!StackIsEmpty(S))
        PopStack(S);
}

void DisposeStack( Stack S )
{
    MakeEmptyStack( S );
    free( S );
}

int teststack()
{
    ElementType c;
    Stack S;

    S = CreateStack();
    while( (c=getchar()) != 'D')
        PushStack(c, S);

    while(!StackIsEmpty(S))
    {
        c = PopStack(S);
        printf("%c", c);
    }
        printf("\n");
	DisposeStack(S);
    return 0;
}

ElementType piarBracket(ElementType c)
{
	switch(c)
	{
	case '(':
		return ')';
		break;
	case '[':
		return ']';
		break;
	case '{':
		return '}';
		break;
	default:
		return '\0';
		break;
	}
}

void ChkBracket(void)
{
	ElementType *str = "abcd(efgi{3}";
	ElementType c, ch;
	Stack S;
	S = CreateStack();
	do
	{
		c = *str;
		if (c=='(' || c=='[' || c=='{')
		{
			PushStack(c, S);
		}
		else if (c==')' || c==']' || c=='}')
		{
			if (StackIsEmpty(S))
			{
				printf("pair failed\n");
				return;
			}
			else
			{
				ch = PopStack(S);
				if (c != piarBracket(ch))
				{
					printf("pair failed\n");
					return;
				}
			}
		}
	}while(*str++ != '\0');
	if (!StackIsEmpty(S))
	{
		printf("pair failed\n");
		return;
	}
	printf("pair OK\n");
}
