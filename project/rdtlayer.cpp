#include "rdtlayer.h"
#include <iostream>
#include "packet.h"
#include <unistd.h>

using namespace std;

RDTLayer::RDTLayer(Socket &sock, RECEIVER_TYPE receiver_type) : sock(sock)
{
    // pick a random number between 0 and half of max sequence number
    next_expected_byte = rand() % (UINT32_MAX / 2);
    state = receiver_type == CLIENT ? CLIENT_START : SERVER_START;
    // handshake();
}

void RDTLayer::start()
{
    while (1)
    {
        receive_packet();
        write_packets();
        read_packet();
        send_packet();
    }
}

void RDTLayer::handshake()
{
    while (state != CONNECTED)
    {
        switch (state)
        {
        case CONNECTED:
        {
            cerr << "Connected" << endl;
            break;
        }
        default:
        {
            cerr << "Default" << endl;
            state = CONNECTED;
            break;
        }
        }
    }
}

void RDTLayer::read_packet()
{
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

    Packet p(0, 0, length, false, false, buf);
    add_packet_to_sending_buffer(p);
}

void RDTLayer::add_packet_to_sending_buffer(Packet p)
{
    if (sending_buffer.size() <= MAX_BUFFER_SIZE)
    {
        sending_buffer.push_back(p);
    }
    else
    {
        cerr << "Sending buffer is full" << endl;
    }
}

void RDTLayer::send_packet()
{
    if (sending_buffer.size() == 0)
        return;
    Packet to_send = sending_buffer[0];
    sending_buffer.erase(sending_buffer.begin());
    uint8_t network_data[PACKET_SIZE];
    int packet_length = to_send.to_network_data(network_data);
    // cerr << " --- Sending packet ---" << endl;
    // cerr << "ack: " << network_data[0] << network_data[1] << network_data[2] << network_data[3] << endl;
    // cerr << "seq: " << network_data[4] << network_data[5] << network_data[6] << network_data[7] << endl;
    // cerr << "length: " << network_data[8] << network_data[9] << endl;
    // cerr << "flags: " << network_data[10] << endl;
    // cerr << "unused: " << network_data[11] << endl;
    // cerr << "payload: " << network_data[12] << network_data[13] << network_data[14] << network_data[15] << endl;
    // cerr << "-----------------------" << endl;
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
        // cerr << "--- Received packet ---" << endl;
        // cerr << "Bytes received: " << bytes_recvd << endl;
        // cerr << "Payload: " << network_data << endl;
        // cerr << "-----------------------" << endl;
        Packet p(network_data, bytes_recvd);
        // p.write_packet_to_stderr();
        receiving_buffer.push_back(p);
    }
    return bytes_recvd;
}

int RDTLayer::write_packets()
{
    // write any acknowledged packets into stdout
    if (receiving_buffer.size() == 0)
        return 0;
    Packet to_write = receiving_buffer[0];
    receiving_buffer.erase(receiving_buffer.begin());
    write(1, to_write.get_payload(), to_write.get_length());
    return 1;
}
