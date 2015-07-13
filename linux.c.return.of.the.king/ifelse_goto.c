#include <stdio.h>

int main(void)
{
    int n=1;

    if(n == 2)
        goto trueCon;
     
    printf("n is not two\n");
    goto done;
trueCon:
    printf("n is two\n");
    goto done;
done:
    return 0;
}
