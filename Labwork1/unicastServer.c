#include "Nets.h"

#define PORT "2001"

int main(int argc, char* argv[])
{
	char buf[BUFSIZE], ip[INET_ADDRSTRLEN];
	sockaddr clientAddr;
	int sockfd;
	socklen_t len;

    if ((sockfd = makeUDPSocket(NULL, PORT)) == -1)
	{
		printf("Client: %s\n", getErrorMsg());
		return -1;
	}
	
	len = sizeof (clientAddr);
	recvfrom(sockfd, buf, BUFSIZE, 0, &clientAddr, &len);
	printf("Server: client's time: %s\n", buf);
	
	memset(buf, 0, sizeof(buf));
	getCurrentTime(buf, BUFSIZE);	
	sendto(sockfd, buf, strlen(buf), 0, &clientAddr, len);
	
	close(sockfd);
	
	return 0;
}
