#include "rdtlayer.h"
#include <iostream>
#include "packet.h"
#include <unistd.h>

using namespace std;

RDTLayer::RDTLayer(Socket &sock, RECEIVER_TYPE receiver_type) : sock(sock)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // seq = rand() % (UINT32_MAX / 2);
    // next_byte_to_receive = UINT32_MAX;
    seq = 0;
    next_byte_to_receive = 0;

    cerr << "Initial sequence number: " << std::dec << seq << endl;
    next_byte_to_send = seq;
    cerr << "Initial next_byte_to_send: " << std::dec << next_byte_to_send << endl;
    state = receiver_type == CLIENT ? CLIENT_START : SERVER_START;
    // handshake();
}

void RDTLayer::start()
{
    while (1)
    {
        receive_packet();
        // update_receiving_buffer();
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
        throw std::runtime_error("Sending buffer is full");
    }

    sending_buffer.push_back(p);
}

void RDTLayer::add_packet_to_receiving_buffer(Packet p)
{
    // add packet to receiving buffer (if it is not a dedicated ack packet)
    if (receiving_buffer.size() >= MAX_BUFFER_SIZE)
    {
        cerr << "attempting to add packet to receiving full buffer" << endl;
        cerr << "buffer size: " << receiving_buffer.size() << endl;
        throw std::runtime_error("Receiving buffer is full");
    }

    // don't add ack packets with no payload to receiving buffer
    if (p.is_ack_set() && p.get_length() == 0)
        return;

    receiving_buffer.push_back(p);
}

void RDTLayer::send_packet()
{
    uint8_t network_data[PACKET_SIZE];
    int packet_length;

    // if no packets, just send an ack
    if (sending_buffer.size() == 0)
    {
        // WHAT SHOULD SEQ BE?
        Packet ack_packet(next_byte_to_receive, seq, 0, true, false, nullptr);
        packet_length = ack_packet.to_network_data(network_data);
        cerr << "SENDING DEDICATED ACK PACKET: " << endl;
        ack_packet.write_packet_to_stderr();
    }

    // send the first packet in the buffer
    else
    {
        Packet to_send = sending_buffer[0];
        sending_buffer.erase(sending_buffer.begin());
        packet_length = to_send.to_network_data(network_data);
        cerr << "SENDING NORMAL PACKET: " << endl;
        to_send.write_packet_to_stderr();
    }

    sock.send_to_socket(network_data, packet_length);
}

int RDTLayer::receive_packet()
{
    // attempt to read from socket
    if (receiving_buffer.size() >= MAX_BUFFER_SIZE)
    {
        cerr << "Receiving buffer is full; not reading from socket" << endl;
        return 0;
    }

    uint8_t network_data[PACKET_SIZE];
    int bytes_recvd = sock.read_from_socket(network_data, PACKET_SIZE);
    if (bytes_recvd > 0)
    {
        Packet p(network_data, bytes_recvd);
        cerr << "RECEIVED PACKET: " << endl;
        cerr << "bytes recieved: " << bytes_recvd << endl;
        p.write_packet_to_stderr();

        if (p.is_ack_set())
            update_sending_buffer(p.get_ack());
        add_packet_to_receiving_buffer(p);
        update_receiving_buffer();
    }
    return bytes_recvd;
}

void RDTLayer::update_receiving_buffer()
{
    // write all packets with seq < next_byte_to_receive
    sort(receiving_buffer.begin(), receiving_buffer.end());

    int packets_written = 0;
    for (vector<Packet>::iterator it = receiving_buffer.begin(); it != receiving_buffer.end();)
    {
        Packet p = *it;
        if (p.get_seq() == next_byte_to_receive)
        {
            write(1, p.get_payload(), p.get_length());
            it = receiving_buffer.erase(it);
            next_byte_to_receive += p.get_packet_size();
            packets_written++;
        }
        else
        {
            break;
        }
    }
    if (packets_written > 0)
        cerr << "Packets written: " << packets_written << endl;
}

void RDTLayer::update_sending_buffer(uint32_t ack_number)
{
    if (ack_number <= next_byte_to_send)
        return;

    cerr << "updating next_byte_to_send to " << ack_number << endl;
    next_byte_to_send = ack_number;
    sort(sending_buffer.begin(), sending_buffer.end());
    // remove all packets with seq < next_byte_to_send
    for (vector<Packet>::iterator it = sending_buffer.begin(); it != sending_buffer.end();)
    {
        Packet p = *it;
        if (p.get_seq() < next_byte_to_send)
            it = sending_buffer.erase(it);
        else
            break;
    }
    cerr << "sending buffer size: " << sending_buffer.size() << endl;
}