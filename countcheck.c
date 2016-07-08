#include <stdio.h>
#include <stdlib.h>

#define U8 unsigned char
#define U16 unsigned short

U8 CountCheck(U8 *_data, U16 _len)
{
    U8 cs = 0;
    U16 i;
    for(i=0;i<_len;i++, _data++)
        cs += *_data;
    
    printf("cs: %02X\n", cs);
    return ((~cs) + 1);
}

int main()
{
    unsigned char data[] = {0x1b, 0x2c, 0xfb, 0x5f, 0x6c};
    printf("%02X\n", CountCheck(data, sizeof(data)));
    return 0;
}
