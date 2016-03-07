#include <stdio.h>
#include <time.h>
int main()
{
    time_t now;   
    struct tm *timenow;   
    char strtemp[255];   
      
    time(&now);   
    timenow = localtime(&now);   
    printf("recent time is : %s\n", asctime(timenow));

    return 0;
}
