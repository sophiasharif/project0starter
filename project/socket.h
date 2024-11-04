#ifndef SOCKET_H
#define SOCKET_H
#include <arpa/inet.h>

class Socket
{
    int sockfd;
    struct sockaddr_in other_addr;

public:
    Socket(int port);
    int get_sockfd() { return sockfd; }
};

#endif