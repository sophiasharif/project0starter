#ifndef RDTLAYER_H
#define RDTLAYER_H

#include "socket.h"
#include "packet.h"
#include <vector>

#define PACKET_SIZE sizeof(packet)

enum RECEIVER_TYPE
{
    CLIENT,
    SERVER
};

class RDTLayer
{
public:
    RDTLayer(Socket &sock, RECEIVER_TYPE receiver_type);
    void send_packet();
    int receive_packet();
    int write_packets();

private:
    Socket &sock;
    std::vector<Packet> packet_buffer;
    uint32_t next_expected_byte;
    RECEIVER_TYPE receiver_type;
};

#endif