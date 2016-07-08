#include <stdio.h>

#define BCD_TO_HEX(x) (((x)/0x10)*0x0A+((x)%0x10))
#define HEX_TO_BCD(x) (((x)/0x0A)<<4 | ((x)%0x0A))

int main()
{
    char s = 31;
    char i = HEX_TO_BCD(s);
    printf("s: %d, to hex: %02x\n", s,i);
    return 0;
}
