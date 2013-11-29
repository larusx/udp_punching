#ifndef _PUNCH_H
#define _PUNCH_H

#include "base.h"
#define sockaddrlen (sizeof(struct sockaddr_in))

typedef struct endpoint endpoint_t;

struct endpoint{
	int fd;
	struct sockaddr_in addr;
};

endpoint_t* get_tcp_endpoint( const char* ip, int port );

#endif
