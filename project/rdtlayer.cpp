#include "rdtlayer.h"
#include <iostream>
#include "packet.h"
#include <unistd.h>

using namespace std;

RDTLayer::RDTLayer(Socket &sock, RECEIVER_TYPE receiver_type) : sock(sock)
{
    // pick a random number between 0 and half of max sequence number
    srand(static_cast<unsigned int>(time(nullptr)));
    seq = rand() % (UINT32_MAX / 2);
    cerr << "Initial sequence number: " << std::dec << seq << endl;
    next_byte_to_send = seq;
    next_byte_to_receive = UINT32_MAX;
    state = receiver_type == CLIENT ? CLIENT_START : SERVER_START;
    // handshake();
}

void RDTLayer::start()
{
    while (1)
    {
        receive_packet();
        write_packets();
        create_packet_from_input();
        send_packet();
    }
}

void RDTLayer::create_packet_from_input()
{
    if (sending_buffer.size() == MAX_BUFFER_SIZE)
        return; // buffer is full; don't read from stdin

    uint8_t buf[MSS];
    int length = read(0, buf, MSS);
    // if length < 0, don't send anything
    if (length == -1)
        return;

    if (length > MSS)
    {
        cerr << "Packet length exceeds maximum segment size" << endl;
        return;
    }

    Packet p(0, seq, length, false, false, buf);
    seq += p.get_packet_size();
    add_packet_to_sending_buffer(p);
}

void RDTLayer::add_packet_to_sending_buffer(Packet p)
{
    if (sending_buffer.size() >= MAX_BUFFER_SIZE)
    {
        cerr << "attempting to add packet to sending full buffer" << endl;
        cerr << "buffer size: " << sending_buffer.size() << endl;
        throw std::runtime_error("Buffer is full");
    }

    sending_buffer.push_back(p);
}

void RDTLayer::send_packet()
{
    if (sending_buffer.size() == 0)
        return;
    Packet to_send = sending_buffer[0];
    sending_buffer.erase(sending_buffer.begin());
    uint8_t network_data[PACKET_SIZE];
    int packet_length = to_send.to_network_data(network_data);
    cerr << "SENDING PACKET: " << endl;
    to_send.write_packet_to_stderr();
    sock.send_to_socket(network_data, packet_length);
}

int RDTLayer::receive_packet()
{
    // attempt to read from socket
    uint8_t network_data[PACKET_SIZE];
    // next step: split into multiple packets if necessary ?
    int bytes_recvd = sock.read_from_socket(network_data, PACKET_SIZE);
    if (bytes_recvd > 0)
    {
        Packet p(network_data, bytes_recvd);
        cerr << "RECEIVED PACKET: " << endl;
        cerr << "bytes recieved: " << bytes_recvd << endl;
        p.write_packet_to_stderr();
        receiving_buffer.push_back(p);
    }
    return bytes_recvd;
}

int RDTLayer::write_packets()
{
    // write all packets with seq < next_byte_to_receive
    sort(receiving_buffer.begin(), receiving_buffer.end());

    int packets_written = 0;
    for (vector<Packet>::iterator it = receiving_buffer.begin(); it != receiving_buffer.end();)
    {
        Packet p = *it;
        if (p.get_seq() < next_byte_to_receive)
        {
            write(1, p.get_payload(), p.get_length());
            it = receiving_buffer.erase(it);
            packets_written++;
        }
        else
        {
            break;
        }
    }
    if (packets_written > 0)
        cerr << "Packets written: " << packets_written << endl;
    return packets_written;

    // if (receiving_buffer.size() == 0)
    //     return 0;
    // Packet to_write = receiving_buffer[0];
    // receiving_buffer.erase(receiving_buffer.begin());
    // write(1, to_write.get_payload(), to_write.get_length());
    // return 1;
}
