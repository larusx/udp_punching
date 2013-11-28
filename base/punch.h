#ifndef _PUNCH_H
#define _PUNCH_H

#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>

#define sockaddrlen (sizeof(struct sockaddr_in))

typedef struct endpoint endpoint_t;

struct endpoint{
	int fd;
	struct sockaddr_in addr;
};

endpoint_t* get_udp_endpoint( const char* ip, int port );

#endif
