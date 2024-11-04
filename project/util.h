#include <sys/socket.h>
#include <arpa/inet.h>

extern int BUF_SIZE;
int read_from_socket(int sockfd, char server_buf[BUF_SIZE], struct sockaddr_in serveraddr, socklen_t address_size);