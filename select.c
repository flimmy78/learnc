#include <sys/select.h>

fd_set *fdset;
int fd;
void FD_ZERO(fdset);
void FD_SET(fd, fdset);
void FD_CLR(fd, fdset);
void FD_ISSET(fd, fset);
