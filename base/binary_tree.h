#ifndef _BINARY_TREE
#define _BINARY_TREE
#include "base.h"
#include <stdlib.h>
#include "punch.h"
typedef struct node node_t;
struct node{
	void* data;   
	node_t* left;
	node_t* right;
};

typedef struct tree_node tree_node_t;
struct tree_node{
	node_t* root;
};

/*create the binary tree*/
tree_node_t* create_tree();

/*alloc the node we can imply this with memory pool*/
node_t* alloc_node(void* data);

/*free the node*/
void free_node(node_t* node);

/*insert the node to the binary tree*/
void insert_tree(tree_node_t* tree, endpoint_t* data);
node_t* insert_node(node_t* root, endpoint_t* data);

/*delete the node which has the fd in the tree*/
void delete_tree_node(tree_node_t* root, int fd);

/*inorder print the node to buf*/
void inorder_print_tree_node( node_t* root, int* buf, int* nfds );
#endif
