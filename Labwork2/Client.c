#include "Nets.h"

int main(int argc, char* argv[])
{
	char buf[BUFSIZE];
	int sockfd;

	if (argc < 3)
	{
		printf("Usage: %s \"server_ip\" \"port\"\n", argv[0]);
		printf("example: %s 134.12.1.5 2001",  argv[0]);
	}

	sockfd = makeTCPConnection(argv[1], argv[2]); 	
	
	if (sockfd == -1)
	{
		printf("Client: %s\n", strerror(errno));
		return -1;
	}
	
	getCurrentTime(buf, BUFSIZE);
	send(sockfd, buf, strlen(buf), 0);
	
	recv(sockfd, buf, BUFSIZE, 0);
	printf("Client: server's time %s\n", buf);

	close(sockfd);
	
	return 0;
}
