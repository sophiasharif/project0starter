#ifndef RDTLAYER_H
#define RDTLAYER_H

#include "socket.h"
#include "packet.h"
#include <vector>

#define PACKET_SIZE sizeof(packet)

class RDTLayer
{
public:
    RDTLayer(Socket &sock);
    void send_packet();
    int receive_packet();
    int write_packets();

private:
    Socket &sock;
    std::vector<Packet> packet_buffer;
};

#endif