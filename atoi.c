#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char* binstr = "0x4B";
    char* endptr;
    int integer = strtol(binstr, &endptr, 16);
    int j = strtol("0x5c", &endptr, 16);
    printf("hex: 0x%02X, dec: %d \n", integer, integer);
    printf("hex: 0x%02X, dec: %d \n", j, j);
    printf("%d\n", '\0');
    return 0;
}
