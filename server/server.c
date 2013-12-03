/*
 * UDP punching server, serve as a P2P server
 */
#include "../base/base.h"
#include "../base/punch.h"
#include "../base/binary_tree.h"
#include "../base/hash.h"
#include "handle.h"
#include <sys/epoll.h>
#include <fcntl.h>

#define EPOLL_NUM 1024
#define BUFSIZE 1024

menu_t server_menu[]={
	{1,"Find who is online"},
	{2,"Connect to someone"},
	{3,"Logout"},
	{0,NULL}
};
void set_nonblocking(int fd)
{
	int opts = fcntl( fd, F_GETFL );
	fcntl( fd, F_SETFL, opts|O_NONBLOCK);
}
/* 
 * if success return 0, otherwise return 1
 */
int server_init( endpoint_t* server , int type )
{
	int on = 1;
	setsockopt( server->fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int) );
	if( bind( server->fd, (struct sockaddr*)&server->addr, sockaddrlen ) !=0 )
		{perror("bind");return 1;}
	if ( type == TCP_ENDPOINT )
		listen( server->fd, EPOLL_NUM );
	return 0;
}
/* 
 * welcome the accepted_fd
 */
void welcome( endpoint_t* ep )
{
	char message[BUFSIZE];
	int nbytes = sprintf(message,"Welcome! Your ID is %d\n",ep->fd);
	for( int i = 0; server_menu[i].number != 0; i++ )
	{
		nbytes += sprintf(&message[nbytes],"%d . %s\n",server_menu[i].number,server_menu[i].content);
	}
	write( ep->fd, message, nbytes);
}
int main()
{
	int epoll_fd = epoll_create( EPOLL_NUM );
	int nfds; /*event number of ready for read or write*/
	socklen_t accept_addrlen = sockaddrlen ;/*use for accept*/
	struct epoll_event ev;
	struct epoll_event kev[EPOLL_NUM];

	/*create a tree to manage the accepted_fds*/
	tree_node_t* accepted_fds_tree = create_tree();

	/*create a hash table to store p2p client*/
	hash_slot_t** p2p_hash_table = hash_init();

	/*we should free this by ourself*/
	endpoint_t* server = get_endpoint( NULL, 80, TCP_ENDPOINT );
	endpoint_t* punch_service = get_endpoint( NULL, 80, UDP_ENDPOINT );
	endpoint_t* client; 

	if( server_init( server, TCP_ENDPOINT ) != 0 )
		{return 1;}
	
	if( server_init( punch_service, UDP_ENDPOINT ) != 0 )
		{return 1;}

	/*add listen_fd to epoll*/
	ev.events = EPOLLIN;
	ev.data.fd = server->fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server->fd, &ev);
	/*add punch_service_fd to epoll*/
	ev.events = EPOLLIN;
	ev.data.fd = punch_service->fd;
	set_nonblocking(punch_service->fd);
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, punch_service->fd, &ev);
	/* 
	 * main loop
	 */
	while ( 1 ) {
		nfds = epoll_wait(epoll_fd, kev, EPOLL_NUM, -1);
		for(int i=0; i<nfds; i++) {
			/* 
			 * TCP events below
			 * something happen in listen_fd
			 */
			if( kev[i].data.fd == server->fd ) {
				client = get_endpoint( NULL, 0, TCP_ENDPOINT );
				client->fd = accept(server->fd, (struct sockaddr*)&client->addr, &accept_addrlen);
				set_nonblocking(client->fd);
				ev.events = EPOLLIN;
				ev.data.fd = client->fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client->fd, &ev);
				insert_tree(accepted_fds_tree, client);
				welcome(client);
			}
			/*
			 * UDP events below
			 * something happen in punch_service_fd
			 */
			else if( kev[i].data.fd == punch_service->fd ) {
				handle_p2p( kev[i].data.fd, p2p_hash_table ); 
			}
			/*
			 * something happen about read, tcp is used only between server and client
			 */
			else if( kev[i].events & EPOLLIN ) {
				if( handle( kev[i].data.fd, accepted_fds_tree ) != 0 ) {
					int fd = kev[i].data.fd;
					ev.data.fd = fd;
					epoll_ctl( epoll_fd, EPOLL_CTL_DEL, fd, &ev );
					close( fd );
					delete_tree_node( accepted_fds_tree, fd );	
				}
			}

		}
	}
	free(server);
	return 0;
}
