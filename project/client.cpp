#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <openssl/evp.h>
#include <fcntl.h>
#include "util.h"
#include "client_socket.h"

void get_host_and_port(char **hostname, int *port, int argc, char **argv)
{
   if (argc != 3)
   {
      printf("Usage: %s <hostname> <port>\n", argv[0]);
      exit(1);
   }

   *hostname = argv[1];
   *port = atoi(argv[2]);
}

int main(int argc, char **argv)
{
   char *hostname;
   int port;
   get_host_and_port(&hostname, &port, argc, argv);

   ClientSocket socket(port, argv[1]);

   int flags = fcntl(0, F_GETFL, 0);
   fcntl(0, F_SETFL, flags | O_NONBLOCK);

   while (1)
   {

      // Execution will stop here until `1024` is read or termination/error
      char server_buf[1024];

      int bytes_recvd = read_from_socket(socket.get_sockfd(), server_buf, socket.servaddr, sizeof(socklen_t));
      // Error if bytes_recvd < 0 :(
      if (bytes_recvd > 0)
         write(1, server_buf, bytes_recvd);

      /* 3. Send data to server */
      char client_buf[1024];

      int bytes_read = read(0, client_buf, 1024);
      if (bytes_read > 0)
      {
         int did_send = sendto(socket.get_sockfd(), client_buf, bytes_read,
                               // socket  send data   how much to send
                               0, (struct sockaddr *)&(socket.servaddr),
                               // flags   where to send
                               sizeof(socket.servaddr));
         if (did_send < 0)
         {
            fprintf(stderr, "Error sending data to server\n");
            return errno;
         }
      }
      // if (did_send < 0)
      //    return errno;
   }
   return 0;
}

/*
switch(client_state) {
   case CLIENT_START:
      send packet with:
         - sequence number = random number between 0 and half of the max size
         - ack number = 0
         - length = 0
         - flags = 0b10000000 (SYN)
         - payload = NULL
   case CLIENT_AWAIT:
      send packet with:
         - sequence number = SEQ + 1 (client sequence number + 1)
         - ack number = server sequence number + 1
         - flags = 0b01000000 (ACK)
         - length = 0
         - payload = NULL
   case CLIENT_AWAIT_2:
   case CONNECTED:
}

switch(server_state) {
   case SERVER_START:
      send packet with:
         - sequence number = random number between 0 and half of the max size
         - ack number = client sequence number + 1
         - length = 0
         - flags = 0b11000000 (SYN+ACK)
         - payload = NULL
   case SERVER_SYN:
   case SERVER_AWAIT_2:
   case CONNECTED:
}
*/