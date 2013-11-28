/*
 * UDP punching server, serve as a P2P server
 */
#include "../base/punch.h"
#include "../base/binary_tree.h"
#include <sys/epoll.h>
#include <fcntl.h>

#define EPOLL_NUM 1024

typedef struct menu menu_t;
struct menu{
	int number;
	char* content;
};
menu_t server_menu[]={
	{1,"Find who is online"},
	{2,"Connect to someone"},
	{3,"Logout"},
	{0,NULL}
};
void set_nonblocking(int fd)
{
	int opts;
	opts = fcntl( fd, F_GETFL );
	fcntl( fd, F_SETFL, opts|O_NONBLOCK);
}
/* 
 * if success return 0, otherwise return 1
 */
int server_init( endpoint_t* server )
{
	int on = 1;
	setsockopt( server->fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int) );
	if( bind( server->fd, (struct sockaddr*)&server->addr, sockaddrlen ) !=0 )
		{perror("bind");return 1;}
	listen( server->fd, 1024 );
	return 0;
}
/* 
 * welcome the accepted_fd
 */
void welcome( endpoint_t* ep )
{
	char message[1024];
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
	struct epoll_event ev;
	struct epoll_event kev[EPOLL_NUM];

	/*create a tree to manage the accepted_fds*/
	tree_node_t* accepted_fds_tree = create_tree();

	/*we should free this by ourself*/
	endpoint_t* server = get_udp_endpoint( NULL, 0 );
	endpoint_t* client; 

	if( server_init( server ) != 0 )
		{return 1;}

	/*add listen_fd to epoll*/
	ev.events = EPOLLIN;
	ev.data.fd = server->fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server->fd, &ev);
	/* 
	 * main loop
	 */
	while ( 1 )
	{
		nfds = epoll_wait(epoll_fd, kev, EPOLL_NUM, -1);
		for(int i=0; i<nfds; i++)
		{
			/* 
			 * something happen in listen_fd
			 */
			if( kev[i].data.fd == server->fd ) {
				client = get_udp_endpoint( NULL, 0 );
				client->fd = accept(server->fd, (struct sockaddr*)&client->addr, NULL);
				set_nonblocking(client->fd);
				ev.events = EPOLLIN;
				ev.data.fd = client->fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client->fd, &ev);
				insert_tree(accepted_fds_tree, client);
				welcome(client);
			}
			/*
			 * something happen about read
			 */
			else if( kev[i].events & EPOLLIN ){
				//handle()
			}

		}
	}
	free(server);
	return 0;
}
