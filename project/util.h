#include <sys/socket.h>
#include <arpa/inet.h>

extern int BUF_SIZE;
int send_packet(int sockfd, char server_buf[BUF_SIZE], struct sockaddr_in serveraddr);