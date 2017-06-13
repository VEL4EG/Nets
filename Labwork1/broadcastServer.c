#include "Nets.h"

#define PORT 2001

int main(int argc, char* argv[])
{
	char buf[BUFSIZE];
	sockaddr_in addr;
	int sockfd;

	if (argc < 2)
	{
		printf("Usage: %s \"server's number\"n", argv[0]);
		printf("example: %s 1",  argv[0]);
		return -1;
	}
	
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("Server %s: %s\n", argv[1], strerror(errno));
		return -1;
	}

	const int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		printf("Server %s: %s\n", argv[1], strerror(errno));
		close(sockfd);
		return -1;
	}
	
	memset(&addr, 0, sizeof (sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1)
	{
		printf("Server %s: %s\n", argv[1], getErrorMsg());
		close(sockfd);
		return -1;
	}

	memset(buf, 0, sizeof buf);
	recvfrom(sockfd, buf, BUFSIZE, 0, NULL, NULL);
	printf("Server %s: client's time: %s\n", argv[1], buf);

	close(sockfd);
	
	return 0;
}
