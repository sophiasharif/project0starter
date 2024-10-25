#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <openssl/evp.h>
#include <fcntl.h>
#include "util.h"

int main(int argc, char **argv)
{

   if (argc != 3)
   {
      printf("Usage: %s <hostname> <port\n", argv[0]);
      return 1;
   }

   // get hostname and port
   char *hostname = argv[1];
   int port = atoi(argv[2]);

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

   /* 4. Create buffer to store incoming data */
   int BUF_SIZE = 1024;
   char server_buf[BUF_SIZE];
   socklen_t serversize = sizeof(socklen_t); // Temp buffer for recvfrom API

   while (1)
   {

      /* 5. Listen for response from server */
      int bytes_recvd = recvfrom(sockfd, server_buf, BUF_SIZE,
                                 // socket  store data  how much
                                 0, (struct sockaddr *)&serveraddr,
                                 &serversize);
      // Execution will stop here until `BUF_SIZE` is read or termination/error
      // Error if bytes_recvd < 0 :(
      if (bytes_recvd > 0)
         write(1, server_buf, bytes_recvd);

      /* 3. Send data to server */
      char client_buf[BUF_SIZE];
      int bytes_read = read(0, client_buf, 1024);
      int did_send = sendto(sockfd, client_buf, bytes_read,
                            // socket  send data   how much to send
                            0, (struct sockaddr *)&serveraddr,
                            // flags   where to send
                            sizeof(serveraddr));
      // if (did_send < 0)
      //    return errno;
   }
   return 0;
}
