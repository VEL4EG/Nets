#include "Nets.h"

#define PORT "2000"

int main(int argc, char* argv[])
{
	char buf[BUFSIZE];
	sockaddr groupAddr;
	int sockfd;
	socklen_t len;

	if (argc < 3)
	{
		printf("Usage: %s \"group ip\" \"port\"\n", argv[0]);
		printf("example: %s 239.0.0.1 2001",  argv[0]);
		return -1;
	}
	
	if (fillAddr(argv[1], argv[2], (sockaddr_in *) &groupAddr) == -1)
	{
		printf("Client: %s\n", getErrorMsg());
		return -1;
	}
	
    if ((sockfd = makeUDPSocket(NULL, PORT)) == -1)
	{
		printf("Client: %s\n", getErrorMsg());
		return -1;
	}
	
	len = sizeof (groupAddr);
	getCurrentTime(buf, BUFSIZE);
	sendto(sockfd, buf, strlen(buf), 0, &groupAddr, len);

	printf("Client: Message has been sent\n");
	
	close(sockfd);

	return 0;
}
