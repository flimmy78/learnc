#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>

char buf[500000];

int main(int argc,char *argv[])
{
        int ntowrite,nwrite;
        const char *ptr ;
        int flags;
    
        ntowrite = read(STDIN_FILENO,buf,sizeof(buf));
        if(ntowrite <0) 
        {   
                perror("read STDIN_FILENO fail:");
                exit(1);
        }   
        fprintf(stderr,"read %d bytes\n",ntowrite);
    
        if((flags = fcntl(STDOUT_FILENO,F_GETFL,0))==-1)
        {   
                perror("fcntl F_GETFL fail:");
                exit(1);
        }   
        flags |= O_NONBLOCK;
        if(fcntl(STDOUT_FILENO,F_SETFL,flags)==-1)
        {   
                perror("fcntl F_SETFL fail:");
                exit(1);
        }   
    
        ptr = buf;
        while(ntowrite > 0)
        {   
                nwrite = write(STDOUT_FILENO,ptr,ntowrite);
                if(nwrite == -1) 
                {  

                        perror("write file fail:");
                }   
                if(nwrite > 0)
                {   
                        ptr += nwrite;
                        ntowrite -= nwrite;
                }   
        }   
    
        flags &= ~O_NONBLOCK;
        if(fcntl(STDOUT_FILENO,F_SETFL,flags)==-1)
        {   
                perror("fcntl F_SETFL fail2:");
        }   
        return 0;
}