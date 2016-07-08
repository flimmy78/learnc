#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc[],char *argv[])
{
	malloc(1);
	printf("errno = %d\n",errno);
	fprintf(stderr,"stderr\n");
	perror("perror");
	printf("strerror: %s\n",strerror(errno));

	malloc(-1);
	printf("errno = %d\n",errno);
	fprintf(stderr,"stderr\n");
	perror("perror");
	printf("strerror: %s\n",strerror(errno));

	return 0;
}
