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
    Packet(uint8_t network_data[], unsigned long data_length);
    int to_network_data(uint8_t network_data[]);
    uint32_t get_ack();
    uint32_t get_seq();
    uint16_t get_length();
    bool is_ack_set();
    bool is_syn_set();
    uint8_t *get_payload();
    void write_packet_to_stderr();

private:
    packet p;
};

#endif