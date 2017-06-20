#include "Nets.h"

char errorMsg[100];

const char* getErrorMsg() {return errorMsg;}

int makeUDPSocket(const char *name, const char *port)
{
	addrinfo hints, *servInfo, *p;
	int sockfd, gaiResult;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	if (name == NULL)
	hints.ai_flags = AI_PASSIVE; // use my wildcard ip

	if ((gaiResult = getaddrinfo(name, port, &hints, &servInfo)) != 0)
	{
		sprintf(errorMsg, "getaddrinfo error: %s", gai_strerror(gaiResult));
		return -1;
	}
	
	char failurePoint[7];
	for (p = servInfo; p != NULL; p = p->ai_next)
	{
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			strcpy(failurePoint, "socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			strcpy(failurePoint, "bind");
            close(sockfd);
            continue;
		}

        break;
	}

	if (p == NULL)
	{
		sprintf(errorMsg, "%s error: %s", failurePoint, strerror(errno));
		sockfd = -1;
	}
	
	freeaddrinfo(servInfo);
	
	return sockfd;
}

int launchTCPServer(const char *port, int maxQueueSize)
{
	int sockfd;
	sockaddr_in addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1)
	{
		sprintf(errorMsg, "socket error: %s", strerror(errno));
		
		return -1;
	}
	
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (sockaddr *) &addr, sizeof addr) == -1)
	{
		sprintf(errorMsg, "bind error: %s", strerror(errno));
		close(sockfd);

		return -1;
	}
	
	if (listen(sockfd, maxQueueSize) == -1)
	{
		sprintf(errorMsg, "listen error: %s", strerror(errno));
		close(sockfd);

		return -1;
	}

	return sockfd;
}

int makeTCPConnection(const char *ip, const char *port)
{
	int sockfd;
	sockaddr_in addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1)
	{
		sprintf(errorMsg, "socket error: %s", strerror(errno));
		
		return -1;
	}

	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	inet_aton(ip, &addr.sin_addr);
	
	if (connect(sockfd, (sockaddr *) &addr, sizeof addr) == -1)
	{
		sprintf(errorMsg, "connect error: %s", strerror(errno));
		close(sockfd);

		return -1;
	}
	
	return sockfd;
}

void getCurrentTime(char *str, size_t maxLength)
{
	static struct timeval timer;
	
	gettimeofday(&timer, NULL);
	strftime(str, maxLength, "%T.", localtime(&timer.tv_sec));
	sprintf(str + strlen(str), "%ld", timer.tv_usec);
}

int getLocalIp(char *ip)
{
	ifaddrs *addrs, *p;
	
	if (getifaddrs(&addrs) != 0)
	{
		sprintf(errorMsg, "getifaddr erorr: %s\n", strerror(errno));
		return -1;
	}

	for(p = addrs; p != NULL; p = p->ifa_next)
	{
		if (p->ifa_addr->sa_family != AF_INET)
		continue;
		
		inet_ntop(AF_INET, &(((sockaddr_in *) p->ifa_addr)->sin_addr), 
												ip, INET_ADDRSTRLEN);
		
		// check whether addr belongs to the local network
		if (strncmp(ip, "192", 3) == 0)
		break;
	}
	
	freeifaddrs(addrs);

	return 0;
}

int fillAddr(const char *name, const char *port, sockaddr_in *addr)
{
	addrinfo hints, *servInfo;
	int gaiResult;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;

	if ((gaiResult = getaddrinfo(name, port, &hints, &servInfo)) != 0)
	{
		sprintf(errorMsg, "getaddrinfo error: %s", gai_strerror(gaiResult));
		return -1;
	}
	
	memcpy(addr, servInfo->ai_addr, servInfo->ai_addrlen);
	
	freeaddrinfo(servInfo);

	return 0;
}

int getBroadcastAddr(const char *localIp, const char *port, sockaddr_in *bAddr)
{
	char bcastIp[INET_ADDRSTRLEN], *pch;	
	int sockfd;
	
	strcpy(bcastIp, localIp);
	pch = strrchr(bcastIp, '.');
	strcpy(pch+1, "255");

	if (fillAddr(bcastIp, SERV_PORT, bAddr) == -1)
	return -1;
	
	return 0;
}
