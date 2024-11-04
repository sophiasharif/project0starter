#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <openssl/evp.h>
#include <fcntl.h>
#include "util.h"
#include "socket.h"

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

   /* 1. Create socket */
   int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   // use IPv4  use UDP

   // make socket non-blocking (update internal fd flags)
   int flags = fcntl(sockfd, F_GETFL, 0);
   fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

   flags = fcntl(0, F_GETFL, 0);
   fcntl(0, F_SETFL, flags | O_NONBLOCK);

   /* 2. Construct server address */
   struct sockaddr_in serveraddr;
   serveraddr.sin_family = AF_INET; // use IPv4

   if (strcmp(hostname, "localhost") == 0)
   {
      // should this be INADDR_LOOPBACK?
      serveraddr.sin_addr.s_addr = INADDR_ANY;
   }
   else
   {
      serveraddr.sin_addr.s_addr = inet_addr(hostname);
   }
   // Set sending port
   // int SEND_PORT = 8080;
   serveraddr.sin_port = htons(port); // Big endian

   while (1)
   {

      // Execution will stop here until `1024` is read or termination/error
      char server_buf[1024];

      int bytes_recvd = read_from_socket(sockfd, server_buf, serveraddr, sizeof(socklen_t));
      // Error if bytes_recvd < 0 :(
      if (bytes_recvd > 0)
         write(1, server_buf, bytes_recvd);

      /* 3. Send data to server */
      char client_buf[1024];

      int bytes_read = read(0, client_buf, 1024);
      if (bytes_read > 0)
      {
         int did_send = sendto(sockfd, client_buf, bytes_read,
                               // socket  send data   how much to send
                               0, (struct sockaddr *)&serveraddr,
                               // flags   where to send
                               sizeof(serveraddr));
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