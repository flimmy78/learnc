#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char str[20];
    sprintf(str, "%d", 4321);
    printf("%s\n", str);
    return 0;
}
