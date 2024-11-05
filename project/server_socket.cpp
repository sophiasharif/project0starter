#include "server_socket.h"

ServerSocket::ServerSocket(int port) : Socket(port)
{
    bind_socket();
}

void ServerSocket::bind_socket()
{
}