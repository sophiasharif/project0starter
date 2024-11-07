#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include "socket.h"
#include <string>

class ClientSocket : public Socket
{
public:
    ClientSocket(int port, std::string hostname);
    int read_from_socket(uint8_t *buf, size_t length);
    int send_to_socket(uint8_t *buf, size_t length);
};

#endif