#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define double_quote(str)    "#str#"
#define sql_char(len)   varchar(len)

int main()
{
    char *space = {(char)0x20};
    char *str = "hello";
    strcat(str, space);
    strcat(str, "world");
    printf("%d, %x, %s\n", space, space, str);
    sql_char(50)
    return 0;
}
