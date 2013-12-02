#ifndef _PUNCH_H
#define _PUNCH_H

#include "base.h"
#define sockaddrlen (sizeof(struct sockaddr_in))


typedef struct menu menu_t;
struct menu{
	int number;
	char* content;
};

typedef struct endpoint endpoint_t;
struct endpoint{
	int fd;
	struct sockaddr_in addr;
};

/* return endpoint_t if type == 0,means TCP, else means UDP*/

enum{
	TCP_ENDPOINT,
	UDP_ENDPOINT
};

/* used for p2p service */
enum{
	LOGIN = 1,
	FIND,
	SEND,
	LOGOUT
};
endpoint_t* get_endpoint( const char* ip, int port, int type );

#endif
