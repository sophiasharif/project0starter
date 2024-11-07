#ifndef RDTLAYER_H
#define RDTLAYER_H

#include "socket.h"
#include "packet.h"
#include <vector>

#define PACKET_SIZE sizeof(packet)
#define MAX_BUFFER_SIZE 20

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
    void create_packet_from_input();
    void send_packet();
    int receive_packet();
    int write_packets();
    void start();

private:
    Socket &sock;
    std::vector<Packet> sending_buffer;
    std::vector<Packet> receiving_buffer;
    uint32_t next_byte_to_receive; // my ack number that I'm sending
    uint32_t next_byte_to_send;    // last ack number I received
    uint32_t seq;                  // next byte to add to sending buffer
    RDTLAYER_STATE state;
    void add_packet_to_sending_buffer(Packet p);
};

#endif