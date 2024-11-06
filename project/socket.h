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
    static const int BUF_SIZE = 1024;
    virtual void read_from_socket(char *buf, size_t length) = 0;
    virtual void send_to_socket(char *buf, size_t length) = 0;

protected:
    int port;
    int sockfd;
};

#endif