#include "pubch.h"
#include "binary_tree.h"
#include <sys/epoll.h>
#include <fcntl.h>

#define EPOLL_NUM 1024
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
int main()
{
	int epoll_fd = epoll_create( EPOLL_NUM );
	int nfds; /*event number of ready for read or write*/
	struct epoll_event ev;
	struct epoll_event kev[EPOLL_NUM];
	endpoint_t* server = get_udp_endpoint( NULL, 0 );
	endpoint_t* client = get_udp_endpoint( NULL, 0 );
	if( server_init( server ) != 0 )
		{free(server);return 1;}
	ev.events = EPOLLIN;
	ev.data.fd = server->fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server->fd, &ep);
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
				client->fd = accept(server->fd, (struct sockaddr*)&client->addr, NULL);
				set_nonblocking(client->fd);
				ev.events = EPOLLIN;
				ev.data.fd = client->fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client->fd, &ev);
			}
			/*
			 * something happen about read
			 */
			else if( kev[i].events & EPOLLIN ){
					//TODO
			}

		}
	}
	free(server);
	return 0;
}
