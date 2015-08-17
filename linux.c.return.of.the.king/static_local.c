#include <stdio.h>
#include <stdlib.h>

int output(char *file_name)
{
    FILE *fp;
    char buf[MAX];
    static int count = 0;
    fp = fopen(file_name, "r");
    if(fp==NULL){
        perror("fail to open");
        return -1;
    }

    while(fgets(fp, buf, MAX) != NULL){
        n = strlen(buf);
        buf[n-1] = '\0';
        printf("%s\n", buf);
        if(count++ % 5 == 0)
            printf("\n");
    }
    fclose(fp);
    return 0;
}

