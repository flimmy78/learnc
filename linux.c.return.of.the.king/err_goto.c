#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int convert(void)
{
    FILE *fp;
    struct stat statbuf;
    char *p, *q;

    int n;
    int res = -1;
    if(stat("text.txt", &statbuf) == -1){
        perror("fail to get stat");
    }
    fp = fopen("text.txt", "rb");

    if(fp == NULL){
        perror("fail to open");
        return res;
    }
    
    p = (char*)malloc(sizeof(char) * (statbuf.st_size + 1));

    if(p == NULL){
        perror("fail to malloc");
        goto memory_err;
    }

    n = fread(p, sizeof(char), statbuf.st_size, fp);
    if(n == -1){
        perror("fail to read");
        goto read_err;
    }

    *(p+n) = '\0';
    q = p;
    while(*q != '\0'){
        if(('a' <= *q) && (*q <= 'z'))
            *q -= 32;
        printf("%c", *q);
        q++;
    }
    printf("\n");
    res = 0;
read_err:
    free(p);
memory_err:
    fclose(fp);

    return res;
}

int main(void)
{
    if(convert() == -1)
        printf("fail to convert\n");

    return 0;
}
