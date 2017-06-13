#include "Nets.h"

#define PORT "2000"

int main(int argc, char* argv[])
{
	char buf[BUFSIZE];
	sockaddr_in bcastAddr;
	int sockfd;
	socklen_t len;

	if (argc < 2)
	{
		printf("Usage: %s \"port\"\n", argv[0]);
		printf("example: %s 2001",  argv[0]);
		return -1;
	}
	
	memset(&bcastAddr, 0, sizeof (sockaddr_in));
	bcastAddr.sin_family = AF_INET;
	bcastAddr.sin_port = htons(atoi(argv[1]));
	bcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    if ((sockfd = makeUDPSocket(NULL, PORT)) == -1)
	{
		printf("Client: %s\n", getErrorMsg());
		return -1;
	}
	
	const int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1)
	{
		printf("Client: %s: %s\n", argv[1], strerror(errno));
		close(sockfd);
		return -1;
	}
	
	len = sizeof (bcastAddr);
	getCurrentTime(buf, BUFSIZE);
	sendto(sockfd, buf, strlen(buf), 0, (sockaddr *)&bcastAddr, len);

	printf("Client: Message has been sent\n");
	
	close(sockfd);

	return 0;
}
