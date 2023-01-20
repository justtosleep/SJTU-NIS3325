#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

# define IP "127.0.0.1"
# define PORT 8000
# define nbytes 1024

void syserr(const char *errstr)
{
    perror(errstr);
    exit(1);
}

void fatal(const char *errstr)
{
    perror(errstr);
    exit(2);
}
