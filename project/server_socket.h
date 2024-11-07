#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "socket.h"

class ServerSocket : public Socket
{
public:
    ServerSocket(int port);
    int read_from_socket(uint8_t *buf, size_t length);
    int send_to_socket(uint8_t *buf, size_t length);
    struct sockaddr_in clientaddr;
    int client_connected;

private:
    void bind_socket();
};

#endif