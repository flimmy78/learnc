
#include  <time.h> 
#include  <stdio.h> 
void main() 
{      
    time_t timep;      
    time (&timep);      
    printf("%s",asctime(gmtime(&timep))); 
}

