#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "socket.h"

class ServerSocket : public Socket
{
public:
    ServerSocket(int port);

private:
    void bind_socket();
};

#endif