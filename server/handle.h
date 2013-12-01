#ifndef _HANDLE_H
#define _HANDLE_H
#include "../base/base.h"
#include "../base/binary_tree.h"
#include "../base/hash.h"
/* handle the tcp fd*/
int handle( int accepted_fd, tree_node_t* accepted_fds_tree );

/* functions */
void find_who_is_online( int fd, tree_node_t* accepted_fds_tree );

/* unsuppoted functions */
void return_error_option( int fd );

/* handle p2p service*/
void handle_p2p( int fd, hash_slot_t** hash_table ); 
#endif
