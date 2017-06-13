#include "Nets.h"

#define PORT "2000"

int main(int argc, char* argv[])
{
	char buf[BUFSIZE];
	sockaddr serverAddr;
	int sockfd;
	socklen_t len;

	if (argc < 3)
	{
		printf("Usage: %s \"server ip\" \"port\"\n", argv[0]);
		printf("example: %s 134.12.1.5 200",  argv[0]);
		return -1;
	}

	if (fillAddr(argv[1], argv[2], (sockaddr_in *) &serverAddr) == -1)
	{
		printf("Client: %s\n", getErrorMsg());
		return -1;
	}
	
    if ((sockfd = makeUDPSocket(NULL, PORT)) == -1)
	{
		printf("Client: %s\n", getErrorMsg());
		return -1;
	}
	
	len = sizeof (serverAddr);
	getCurrentTime(buf, BUFSIZE);
	sendto(sockfd, buf, strlen(buf), 0, &serverAddr, len);
	
	recvfrom(sockfd, buf, BUFSIZE, 0, NULL, NULL);
	printf("Client: server's time %s\n", buf);
	
	close(sockfd);
	
	return 0;
}
