#ifndef SOCKET_H
#define SOCKET_H
#include <arpa/inet.h>

class Socket
{
public:
    Socket(int port);
    virtual ~Socket();
    int get_sockfd() { return sockfd; }
    struct sockaddr_in servaddr;

protected:
    int port;
    int sockfd;
};

#endif