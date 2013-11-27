#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

#define sockaddrlen (sizeof(struct sockaddr_in))
#define BUFSIZE 100
typedef struct endpoint endpoint_t;

struct endpoint{
	int fd;
	struct sockaddr_in addr;
};

endpoint_t* get_udp_endpoint( const char* ip, int port )
{
	endpoint_t* ep = ( endpoint_t* )malloc( sizeof( endpoint_t ) );
	memset( &ep->addr, 0, sockaddrlen );
	ep->fd = socket( AF_INET, SOCK_STREAM, 0 );
	ep->addr.sin_family = AF_INET;
	if ( port != 0)
		ep->addr.sin_port = htons( port );
	if ( ip != NULL )
		ep->addr.sin_addr.s_addr = inet_addr( ip ); 
	else
		ep->addr.sin_addr.s_addr = INADDR_ANY;
	return ep;
}
/* 
 * if success return 0, otherwise return 1
 
 */
int login( endpoint_t* server )
{
	if ( connect( server->fd, ( struct sockaddr* )&server->addr, sockaddrlen ) !=0 )
		{ perror("connect");return 1; }
	char buf[BUFSIZE];
	int nbytes = read( server->fd, buf, BUFSIZE );
	buf[nbytes] = 0;
	printf("%s\n",buf);
	return 0;
}

int main()
{
	endpoint_t* server = get_udp_endpoint( "211.211.211.211", 8888 );
	char sendbuf[BUFSIZE];
	char recvbuf[BUFSIZE];
	int nbytes;
	if( login( server ) != 0)
		return 1;
	/*main loop*/
	while( 1 )
	{
		nbytes = scanf("%s",sendbuf);

	}
	return 0;
}
