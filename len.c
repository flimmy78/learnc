#include <stdio.h>

typedef unsigned char U8;
typedef struct{
    U8 u8second;//秒
    U8 u8minute;//分
    U8 u8hour;//时
    U8 u8day;//日
    U8 u8month;//月
    U8 u8year;//年              
} sys_time_str;

int main(void)
{
    printf("sizeof sys_time_str is: %lu\n", sizeof(sys_time_str));
    return 0;
}
