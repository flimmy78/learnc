#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    FILE *fd;
    char b;
    char file_buf[20];
    int i, len;
    if ((fd = fopen(argv[1], "rb")) == NULL) {
        printf("open file err\n");
        return 1;
    }
    while((len=fread(file_buf, 1, 16, fd ) > 0)) {
        for(i=0;i<len;i++)
            printf("%c", file_buf[i]);
    }
    fclose(fd);
    printf("\n");
    write(1, "fuck\n", 6);
    return 0;
}
