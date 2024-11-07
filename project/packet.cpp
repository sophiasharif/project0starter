#include "packet.h"
#include <cstring>
#include <algorithm>
#include <iostream>

using namespace std;

#define HEADER_SIZE (sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint8_t) + sizeof(uint8_t))

Packet::Packet(uint32_t ack, uint32_t seq, uint16_t length, bool ack_bit, bool syn_bit, uint8_t *payload)
{
    p.ack = ack;
    p.seq = seq;
    p.length = length;
    p.flags = 0 | (ack_bit << 1) | syn_bit;
    memcpy(p.payload, payload, min(length, static_cast<uint16_t>(MSS)));
}

Packet::Packet(uint8_t network_data[], unsigned long data_length)
{
    const uint8_t *ptr = network_data;

    if (data_length < HEADER_SIZE)
    {
        throw std::runtime_error("Data length is too short to form a valid packet");
    }

    // Read ack number (uint32_t)
    uint32_t net_ack;
    memcpy(&net_ack, ptr, sizeof(uint32_t));
    p.ack = ntohl(net_ack);
    ptr += sizeof(uint32_t);

    // Read seq number (uint32_t)
    uint32_t net_seq;
    memcpy(&net_seq, ptr, sizeof(uint32_t));
    p.seq = ntohl(net_seq);
    ptr += sizeof(uint32_t);

    // Read length (uint16_t)
    uint16_t net_length;
    memcpy(&net_length, ptr, sizeof(uint16_t));
    p.length = ntohs(net_length);
    ptr += sizeof(uint16_t);

    // Read flags (uint8_t)
    p.flags = *ptr;
    ptr += sizeof(uint8_t);

    // Read unused (uint8_t)
    p.unused = *ptr;
    ptr += sizeof(uint8_t);

    // Calculate payload length
    size_t payload_length = data_length - HEADER_SIZE;
    if (payload_length > MSS || p.length > MSS)
    {
        cerr << "Computed payload length: " << payload_length << endl;
        cerr << "Packet length: " << p.length << endl;
        throw std::runtime_error("Invalid payload length");
    }

    // Copy payload
    memcpy(p.payload, ptr, payload_length);
}

int Packet::to_network_data(uint8_t network_data[])
{
    uint8_t *ptr = network_data;

    // Write ack number (convert to network byte order)
    uint32_t net_ack = htonl(p.ack);
    memcpy(ptr, &net_ack, sizeof(uint32_t));
    ptr += sizeof(uint32_t);

    // Write seq number (convert to network byte order)
    uint32_t net_seq = htonl(p.seq);
    memcpy(ptr, &net_seq, sizeof(uint32_t));
    ptr += sizeof(uint32_t);

    // Write length (convert to network byte order)
    uint16_t net_length = htons(p.length);
    memcpy(ptr, &net_length, sizeof(uint16_t));
    ptr += sizeof(uint16_t);

    // Write flags
    *ptr = p.flags;
    ptr += sizeof(uint8_t);

    // Write unused field
    *ptr = p.unused;
    ptr += sizeof(uint8_t);

    // Write payload (ensure not to exceed `p.length` or MSS)
    uint16_t payload_length = std::min(p.length, static_cast<uint16_t>(MSS));
    memcpy(ptr, p.payload, payload_length);
    ptr += payload_length;

    // Return total packet length for use in send_to_socket
    return ptr - network_data;
}

uint32_t Packet::get_ack() const
{
    return p.ack;
}
uint32_t Packet::get_seq() const
{
    return p.seq;
}
uint16_t Packet::get_length() const
{
    return p.length;
}
bool Packet::is_ack_set() const
{
    return p.flags & 0x2;
}
bool Packet::is_syn_set() const
{
    return p.flags & 0x1;
}
uint8_t *Packet::get_payload()
{
    return p.payload;
}

void Packet::write_packet_to_stderr()
{
    cerr << "-- Packet --" << endl;
    cerr << "   ack: " << p.ack << endl;
    cerr << "   seq: " << p.seq << endl;
    cerr << "   length: " << p.length << endl;
    cerr << "   ACK flag: " << is_ack_set() << endl;
    cerr << "   SYN flag: " << is_syn_set() << endl;
    cerr << "   unused: " << p.unused << endl;
    cerr << "   payload: " << endl
         << "   ";
    for (int i = 0; i < p.length; i++)
    {
        cerr << p.payload[i];
    }
    cerr << "------------" << endl;
}