#include "handle.h"
#include "../base/binary_tree.h"

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
	write( fd, sendbuf, nbytes );
}

/* unsuppoted function */
void return_error_option( int fd )
{
	char *sendbuf = "You should enter the right number!";
	int sendlen = strlen(sendbuf);
	write( fd, sendbuf, sendlen );
}
/*
 * handle the client tcp fd
 */
void handle( int accepted_fd, tree_node_t* accepted_fds_tree )
{
	char recvbuf[BUFSIZE];
	/*int nbytes = */
	read( accepted_fd, recvbuf, BUFSIZE);
	int option_number = recvbuf[0]-'0';
	switch(option_number){
		case 1:
			find_who_is_online(accepted_fd, accepted_fds_tree);break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		default :
			return_error_option(accepted_fd);
	}
}
