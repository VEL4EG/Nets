#include "Nets.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

enum ConnectionMode
{
	unicast,
	multicast,
	broadcast,
	TCP,
	FTP
};

typedef enum ConnectionMode ConnectionMode;

void start(ConnectionMode mode, char *port, char *ip);

int main()
{
	pid_t pid;
	char fileName[10], ip[INET_ADDRSTRLEN], port[] = "2001";
	
	if (getLocalIp(ip) == -1)
	{
		printf("%s\n", getErrorMsg());
		return -1;
	}

	printf("Labwork1\n");
	
	printf("Unicast\n");
	start(unicast, port, ip);

	printf("Multicast\n");
	start(multicast, port, "239.0.0.1");

	printf("Broadcast\n");
	start(broadcast, port, NULL); 
	printf("Labwork2\n");
	start(TCP, port, ip); 
}

void start(ConnectionMode mode, char *port, char *ip)
{
	pid_t pid;
	char serverFileName[30], clientFileName[30], serverNumber[5];
	int serversCount;
	char *clientArgv[4] = {clientFileName, ip, port, NULL};
	char *serverArgv[4] = {serverFileName, NULL, NULL, NULL};
	char *const envp[] = {NULL};

	switch(mode)
	{
		case unicast:
		{
			strcpy(serverFileName, "./Labwork1/unicastServer");
			strcpy(clientFileName, "./Labwork1/unicastClient");
			serversCount = 1;
		}
		break;
		
		case multicast:
		{
			strcpy(serverFileName, "./Labwork1/multicastServer");
			strcpy(clientFileName, "./Labwork1/multicastClient");
			serverArgv[2] = ip;
			serversCount = 3;
		}
		break;
		
		case broadcast:
		{
			strcpy(serverFileName, "./Labwork1/broadcastServer");
			strcpy(clientFileName, "./Labwork1/broadcastClient");
			serversCount = 3;
			clientArgv[1] = port;
			clientArgv[2] = NULL;
		}
		break;
		
		case TCP:
		{
			strcpy(serverFileName, "./Labwork2/Server");
			strcpy(clientFileName, "./Labwork2/Client");
			serversCount = 1;
		}
		break;
	}
	
	for (int i = 0; i < serversCount; i++)
	{
		switch (fork())
		{
			case 0:
			{
				if (mode != unicast)
				{
					sprintf(serverNumber, "%d", i+1);
					serverArgv[1] = serverNumber;
				}
			
				execve(serverFileName, serverArgv, envp);
				printf("execve server error: %s\n", strerror(errno));
			}

			case -1:
			{
				printf("fork server error: %s\n", strerror(errno));
				return;
			}
		}
	}

	printf("Wait for servers\n");
	sleep(1);

	switch (fork())
	{
		case 0:
		{
			execve(clientFileName, clientArgv, envp);
			printf("execve client error: %s\n", strerror(errno));
		}

		case -1:
		{
			printf("fork client error: %s\n", strerror(errno));
			return;
		}
	}

	for (int i = 0; i < serversCount + 1; i++)
	wait(NULL);
}
