#ifndef SOCKET_H
#define SOCKET_H
#include <arpa/inet.h>

class Socket
{
    int sockfd;
    struct sockaddr_in my_addr;

public:
    Socket(int port);
};

#endif