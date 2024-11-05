#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "server_socket.h"

int main(int argc, char **argv)
{
   if (argc != 2)
   {
      fprintf(stderr, "Usage: %s <port>\n", argv[0]);
      return 1;
   }
   int port = atoi(argv[1]);
   ServerSocket socket(port);

   // make stdin non-blocking
   int flags = fcntl(0, F_GETFL, 0);
   fcntl(0, F_SETFL, flags | O_NONBLOCK);

   /* 4. Create buffer to store incoming data */
   int BUF_SIZE = 1024;
   char client_buf[BUF_SIZE];
   struct sockaddr_in clientaddr; // Same information, but about client
   socklen_t clientsize = sizeof(clientaddr);

   /* 5. Listen for data from clients */
   int client_connected = 0;

   while (1)
   {
      int bytes_recvd = recvfrom(socket.get_sockfd(), client_buf, BUF_SIZE,
                                 // socket  store data  how much
                                 0, (struct sockaddr *)&clientaddr,
                                 &clientsize);
      // Execution will stop here until `BUF_SIZE` is read or termination/error
      // Error if bytes_recvd < 0 :(
      if (bytes_recvd <= 0 && !client_connected)
         continue;

      client_connected = 1;
      // why do we need these variables?
      /* 6. Inspect data from client */
      char *client_ip = inet_ntoa(clientaddr.sin_addr);
      // "Network bytes to address string"
      int client_port = ntohs(clientaddr.sin_port); // Little endian

      if (bytes_recvd > 0)
      {
         write(1, client_buf, bytes_recvd);
      }
      // read from stdin

      char server_buf[1024];
      int bytes_read = read(0, server_buf, 1024);
      int did_send = sendto(socket.get_sockfd(), server_buf, bytes_read,
                            // socket  send data   how much to send
                            0, (struct sockaddr *)&clientaddr,
                            // flags   where to send
                            sizeof(clientaddr));
      // if (did_send < 0)
      //    return errno;

      // /* 8. You're done! Terminate the connection */
      // close(sockfd);
   }

   /* 7. Send data back to client */
   return 0;
}