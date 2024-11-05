#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include "socket.h"
#include <string>

class ClientSocket : public Socket
{
public:
    ClientSocket(int port, std::string hostname);
    void read_from_socket();
};

#endif