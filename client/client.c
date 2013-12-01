#include "../base/punch.h"

#define BUFSIZE 1024

char* p2p_server = "219.217.151.151";
socklen_t socklen = sockaddrlen;
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
void handle_msg( endpoint_t* remote, char* msg )
{
	sendto( remote->fd, msg, strlen(msg), 0, (struct sockaddr*)&remote->addr, sockaddrlen);
	int len = recvfrom( remote->fd, msg, BUFSIZE, 0, (struct sockaddr*)&remote->addr, &socklen);
	msg[len] = 0;
	printf("%s",msg);
}
void punch_handle_client (void)
{
	endpoint_t* server = get_endpoint( p2p_server, 80, UDP_ENDPOINT );
	printf("========================\n");
	printf("Now you are in p2p mode!\n");
	printf("Please input your name (at most 10 characters) : ");
	char sendbuf[BUFSIZE]; 
	sendbuf[0] = LOGIN;
	scanf("%s",&sendbuf[1]);
  handle_msg( server, sendbuf );
	while ( 1 )
	{
		
	}
}
int main()
{
	endpoint_t* server = get_endpoint( p2p_server, 80, TCP_ENDPOINT );
	char sendbuf[BUFSIZE];
	char recvbuf[BUFSIZE];
	int nbytes;
	if( login( server ) != 0)
		return 1;
	/*main loop*/
	while( 1 )
	{
		nbytes = scanf("%s",sendbuf);
		if ( sendbuf[0] == '2' )
			{punch_handle_client();continue;}
		write( server->fd, sendbuf, nbytes );
		nbytes = read ( server->fd, recvbuf, BUFSIZE );
		recvbuf[nbytes] = 0;
		printf("%s",recvbuf);
	}
	free( server );
	return 0;
}
