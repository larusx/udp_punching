#include "../base/punch.h"

#define BUFSIZE 1024

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
	printf("%s",buf);
	return 0;
}

int main()
{
	endpoint_t* server = get_udp_endpoint( "23.227.187.13", 80 );
	char sendbuf[BUFSIZE];
	char recvbuf[BUFSIZE];
	int nbytes;
	if( login( server ) != 0)
		return 1;
	/*main loop*/
	while( 1 )
	{
		nbytes = scanf("%s",sendbuf);
		write( server->fd, sendbuf, nbytes );
		nbytes = read ( server->fd, recvbuf, BUFSIZE );
		recvbuf[nbytes] = 0;
		printf("%s",recvbuf);
	}
	free( server );
	return 0;
}
