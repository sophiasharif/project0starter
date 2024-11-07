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
    virtual int read_from_socket(uint8_t *buf, size_t length) = 0;
    virtual int send_to_socket(uint8_t *buf, size_t length) = 0;

protected:
    int port;
    int sockfd;
};

#endif