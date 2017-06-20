#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 100
#define SERV_PORT "2000"
#define ATTEMPTS_COUNT 10

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct ifaddrs ifaddrs;
typedef struct addrinfo addrinfo;

int makeUDPSocket(const char *name, const char *port);
int launchTCPServer(const char *port, int maxQueueSize);
int makeTCPConnection(const char *ip, const char *port);
int getLocalIp(char *ip);
const char* getErrorMsg();
int fillAddr(const char *name, const char *port, sockaddr_in *addr);
int getBroadcastAddr(const char *localIp, const char *port, sockaddr_in *bAddr);
void getCurrentTime(char *str, size_t maxLength);
