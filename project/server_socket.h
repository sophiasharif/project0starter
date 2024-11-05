#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "socket.h"

class ServerSocket : public Socket
{
public:
    ServerSocket(int port);
    void read_from_socket();
    struct sockaddr_in clientaddr;
    int client_connected;

private:
    void bind_socket();
};

#endif