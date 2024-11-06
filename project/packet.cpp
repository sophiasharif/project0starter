#include "packet.h"
#include <cstring>
#include <algorithm>

using namespace std;

Packet::Packet(uint32_t ack, uint32_t seq, uint16_t length, bool ack_bit, bool syn_bit, uint8_t *payload)
{
    p.ack = ack;
    p.seq = seq;
    p.length = length;
    p.flags = 0 | (ack_bit << 1) | syn_bit;
    memcpy(p.payload, payload, min(length, static_cast<uint16_t>(MSS)));
}

Packet::Packet(packet network_packet)
{
    p.ack = ntohl(network_packet.ack);
    p.seq = ntohl(network_packet.seq);
    p.length = ntohs(network_packet.length);
    p.flags = network_packet.flags;
    memcpy(p.payload, network_packet.payload, min(p.length, static_cast<uint16_t>(MSS)));
}

packet Packet::to_network_packet()
{
    packet network_packet;
    network_packet.ack = htonl(p.ack);
    network_packet.seq = htonl(p.seq);
    network_packet.length = htons(p.length);
    network_packet.flags = p.flags;
    memcpy(network_packet.payload, p.payload, min(p.length, static_cast<uint16_t>(MSS)));
    return network_packet;
}
uint32_t Packet::get_ack()
{
    return p.ack;
}
uint32_t Packet::get_seq()
{
    return p.seq;
}
uint16_t Packet::get_length()
{
    return p.length;
}
bool Packet::is_ack_set()
{
    return p.flags & 0x2;
}
bool Packet::is_syn_set()
{
    return p.flags & 0x1;
}
uint8_t *Packet::get_payload()
{
    return p.payload;
}