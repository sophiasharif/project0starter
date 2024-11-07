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
    uint32_t get_ack() const;
    uint32_t get_seq() const;
    uint16_t get_length() const;
    uint16_t get_packet_size() const;
    bool is_ack_set() const;
    bool is_syn_set() const;
    uint8_t *get_payload();
    void write_packet_to_stderr();
    bool operator<(const Packet &other) const
    {
        return get_seq() < other.get_seq();
    }

private:
    packet p;
};

#endif