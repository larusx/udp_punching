#include "handle.h"

#define ONLINE_NUMBER 1024
#define BUFSIZE 1024
/*
 * return fd the online users
 */
void find_who_is_online( int fd, tree_node_t* accepted_fds_tree )
{
	int accepted_fds[ONLINE_NUMBER];
	int nfds = 0, nbytes = 0/*bytes will be send*/;
	char sendbuf[BUFSIZE];
	/* the fds will be set in accepted_fds*/
	inorder_print_tree_node( accepted_fds_tree->root, accepted_fds , &nfds );
	for ( int i=0; i<nfds; i++ )
		nbytes += sprintf(&sendbuf[nbytes], "%d ", accepted_fds[i]);
	nbytes += sprintf(&sendbuf[nbytes], "\n");
	write( fd, sendbuf, nbytes );
}

/* unsuppoted function */
void return_error_option( int fd )
{
	char *sendbuf = "You should enter the right number!\n";
	int sendlen = strlen(sendbuf);
	write( fd, sendbuf, sendlen );
}
/*
 * handle the client tcp fd
 */
int handle( int accepted_fd, tree_node_t* accepted_fds_tree )
{
	char recvbuf[BUFSIZE];
	int nbytes = read( accepted_fd, recvbuf, BUFSIZE);
	perror("read");
	if ( nbytes == 0 )
		return 1;/* remote is closed */	
	int option_number = recvbuf[0]-'0';
	switch(option_number){
		case 1:
			find_who_is_online(accepted_fd, accepted_fds_tree);break;
		case 2:
		case 3:
		case 4:
		default :
			return_error_option(accepted_fd);
	}
	return 0;
}
/* 
 * handle p2p client
 */
void handle_p2p( int fd, hash_slot_t** hash_table ) 
{
	char msg[BUFSIZE];
	socklen_t socklen = sockaddrlen;
	int nbytes;
	endpoint_t client;
	endpoint_t* insert_ep;
	hash_slot_t* ret_slot;/*used for SEND*/
	int len = recvfrom( fd, msg, BUFSIZE, 0, (struct sockaddr*)&client.addr, &socklen);
	if ( len == 0 )
		return ;
	switch (msg[0]) {
		case LOGIN:
			insert_ep = (endpoint_t*)malloc(sizeof(endpoint_t));
			memcpy( insert_ep, &client, sizeof(endpoint_t) );
			hash_insert( hash_table, &msg[2], msg[1], insert_ep );
			strcpy( msg, "Success Login!\n" );
			sendto( fd, msg, strlen(msg), 0, (struct sockaddr*)&client.addr, socklen );
			break;
		case FIND:
			nbytes = hash_iter( hash_table, msg );
			sendto( fd, msg, nbytes, 0, (struct sockaddr*)&client.addr, socklen );
			break;
		case SEND:
			ret_slot = hash_find( hash_table, &msg[2], msg[1] );
			memcpy( msg, &ret_slot->ep->addr, sockaddrlen );/*send sockaddr to remote user*/
			sendto( fd, msg, sockaddrlen, 0, (struct sockaddr*)&client.addr, socklen );
			break;
		case LOGOUT:
			hash_delete( hash_table, &msg[2], msg[1] );
			strcpy( msg, "Success Logout!\n");
			sendto( fd, msg, strlen(msg), 0, (struct sockaddr*)&client.addr, socklen );
	}
}
