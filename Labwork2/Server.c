#include "Nets.h"

#define PORT "2001"

int main(int argc, char* argv[])
{
	char buf[BUFSIZE];
	int serverSockfd, clientSockfd;

	serverSockfd = launchTCPServer(PORT, 2);

	if (serverSockfd == -1)
	{
		printf("Server error: %s\n", getErrorMsg());
		return -1;
	}

	clientSockfd = accept(serverSockfd, NULL, NULL);

	if (clientSockfd == -1)
	{
		close(serverSockfd); 
		printf("Server accept error: %s\n", strerror(errno));
		return -1;
	}

	memset(buf, 0, sizeof buf);
	recv(clientSockfd, buf, BUFSIZE, 0);
	printf("Server: client's time: %s\n", buf);
	
	getCurrentTime(buf, BUFSIZE);
	send(clientSockfd, buf, strlen(buf), 0);
	
	// Ждем, чтобы клиент инициировал закрытие соединения, во избежание TIMED_WAIT,
	// которое приведет к невозможности забиндиться некоторое время
	sleep(1);

	close(serverSockfd); 
	close(clientSockfd); 
	
	return 0;
}
