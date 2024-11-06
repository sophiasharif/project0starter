#ifndef PACKET_H
#define PACKET_H
#include <stdint.h>

#define MSS 1012 // MSS = Maximum Segment Size (aka max length)
typedef struct
{
    uint32_t ack;
    uint32_t seq;
    uint16_t length;
    uint8_t flags;
    uint8_t unused;
    uint8_t payload[MSS];
} packet;

class Packet
{
public:
    Packet(uint32_t ack, uint32_t seq, uint16_t length, bool ack_bit, bool syn_bit, uint8_t *payload);
    Packet(packet network_packet);
    packet to_network_packet();
    uint32_t get_ack();
    uint32_t get_seq();
    uint16_t get_length();
    bool is_ack_set();
    bool is_syn_set();
    uint8_t *get_payload();

private:
    packet p;
};

#endif