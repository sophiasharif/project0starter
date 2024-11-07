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

enum RDTLAYER_STATE
{
    CLIENT_START,
    CLIENT_AWAIT,
    CLIENT_AWAIT_2,
    SERVER_START,
    SERVER_SYN,
    SERVER_AWAIT_2,
    CONNECTED
};

class RDTLayer
{
public:
    RDTLayer(Socket &sock, RECEIVER_TYPE receiver_type);
    void read_packet();
    void send_packet();
    int receive_packet();
    int write_packets();

private:
    Socket &sock;
    std::vector<Packet> sending_buffer;
    std::vector<Packet> receiving_buffer;
    uint32_t next_expected_byte;
    RDTLAYER_STATE state;
    void handshake();
};

#endif