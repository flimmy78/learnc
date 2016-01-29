#include <stdio.h>
#include <fcntl.h>
int main(int cont,char *argc[])
{
    close(1);
    open("test.txt", O_RDWR);
    printf("FUCK YOU\n");
    close(1);
    return 0;
}
