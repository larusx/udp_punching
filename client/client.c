#include "../base/punch.h"
#include <pthread.h>

#define BUFSIZE 1024

menu_t p2p_menu[] = {
	{ FIND, "Find who is online" },
	{ SEND, "pick one user to send"},
	{ LOGOUT, "Log out" },
	{ 0, NULL }
};
//char* p2p_server = "23.227.187.13";
char* p2p_server = "127.0.0.1";

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
void* p2p_send_msg(void *arg)
{
	endpoint_t* remote = (endpoint_t*)arg;
	char msg[BUFSIZE];
	printf("Send Msg \n");
	while ( 1 ) {
		memset ( msg, 0, BUFSIZE );
		scanf("%s",msg);
		sendto( remote->fd, msg, strlen(msg), 0, (struct sockaddr*)&remote->addr, sockaddrlen);
	}
}
void handle_p2p_client( endpoint_t* p2p_server, char* msg )
{
	pthread_t tid;
	sendto( p2p_server->fd, msg, strlen(msg), 0, (struct sockaddr*)&p2p_server->addr, sockaddrlen);
	recvfrom( p2p_server->fd, msg, BUFSIZE, 0, (struct sockaddr*)&p2p_server->addr, &socklen);
	endpoint_t remote;
	memcpy( &remote.addr, msg, sockaddrlen );
	remote.fd = p2p_server->fd;/*share the same session with server*/
	pthread_create( &tid, NULL, p2p_send_msg, (void*)&remote );
	printf("Recv Msg \n");
	while ( 1 ) {
		int len = recvfrom( p2p_server->fd, msg, BUFSIZE, 0, (struct sockaddr*)&remote.addr, &socklen);
		msg[len] = 0;
		printf("<<<%s\n",msg);
	} 

}
void punch_handle_client (void)
{
	char name[20];
	memset(name, 0, 20);
	char sendbuf[BUFSIZE]; 
	endpoint_t* server = get_endpoint( p2p_server, 80, UDP_ENDPOINT );
	printf("========================\n");
	printf("Now you are in p2p mode!\n");
	printf("Please input your name (at most 10 characters) : ");
	name[0] = LOGIN;
	scanf("%s",&name[2]);
	name[1] = strlen(&name[2]);
	strcpy(sendbuf, name);
  handle_msg( server, sendbuf );
	for( int i = 0; p2p_menu[i].number  != 0; i++ ) {
		printf("%d. %s\n", p2p_menu[i].number, p2p_menu[i].content);
	}
	while ( 1 ) {
		scanf("%s",sendbuf);
		switch( sendbuf[0]-'0' ) {
			case FIND:
				sendbuf[0] = FIND;
				sendbuf[1] = 0; 
				handle_msg( server, sendbuf );
				break;
			case SEND:
				printf("input name: ");
				memset(sendbuf, 0, BUFSIZE);
				scanf("%s",&sendbuf[2]);
				sendbuf[0] = SEND;
				sendbuf[1] = strlen(&sendbuf[2]);
				handle_p2p_client( server, sendbuf );
				break;
			case LOGOUT:
				name[0] = LOGOUT;
				handle_msg( server, name );
				return;
		}
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
