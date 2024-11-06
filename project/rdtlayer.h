#ifndef RDTLAYER_H
#define RDTLAYER_H

#include "socket.h"
#include "packet.h"

class RDTLayer
{
public:
    RDTLayer(Socket &sock);
    void send_packet(char *buf, int length);
    void receive_packet(char *buf, int length);

private:
    Socket &sock;
};

#endif