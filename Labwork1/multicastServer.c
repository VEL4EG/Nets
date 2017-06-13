#include "Nets.h"

#define PORT "2001"

struct ip_mreqn 
{
	struct in_addr imr_multiaddr; /* IP multicast group
								   address */
	struct in_addr imr_address;   /* IP address of local
								   interface */
	int imr_ifindex;   /* interface index */
};

typedef struct ip_mreqn ip_mreqn;

int main(int argc, char* argv[])
{
	char buf[BUFSIZE];
	sockaddr_in addr;
	int sockfd;

	if (argc < 3)
	{
		printf("Usage: %s \"server's number\" \"group ip\"\n", argv[0]);
		printf("example: %s 1 239.0.0.1",  argv[0]);
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

	if (fillAddr(argv[2], PORT, &addr) == -1)
	{
		printf("Server %s: %s\n", argv[1], getErrorMsg());
		close(sockfd);
		return -1;
	}

	if (bind(sockfd, (sockaddr *) &addr, sizeof(addr)) == -1)
	{
		printf("Server %s: %s\n", argv[1], getErrorMsg());
		close(sockfd);
		return -1;
	}

	struct ip_mreqn mreq;
	inet_aton(argv[2], &(mreq.imr_multiaddr));
	mreq.imr_address.s_addr = htonl(INADDR_ANY);
	mreq.imr_ifindex = 0;

	if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof mreq) == -1)
	{
		printf("Server %s: %s\n", argv[1], strerror(errno));
		close(sockfd);
		return -1;
	}
	
	memset(buf, 0, sizeof buf);
	recvfrom(sockfd, buf, BUFSIZE, 0, NULL, NULL);
	printf("Server %s: client's time: %s\n", argv[1], buf);

	close(sockfd);
	
	return 0;
}
