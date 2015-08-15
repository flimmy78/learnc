#include <stdio.h>

int i;
void f()
{
    int a[100];
    for(i=0; i< 100;i++)
        a[i]=i;
}

void q()
{
    int j;
    int a[100];

    for(j=0;j<100;j++)
        a[j]=j;
}

int main(void)
{
    f();
   // q();

    return 0;
}
