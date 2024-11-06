#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "socket.h"

class ServerSocket : public Socket
{
public:
    ServerSocket(int port);
    void read_from_socket(char *buf, size_t length);
    void send_to_socket(char *buf, size_t length);
    struct sockaddr_in clientaddr;
    int client_connected;

private:
    void bind_socket();
};

#endif