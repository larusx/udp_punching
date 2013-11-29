#include "binary_tree.h"

tree_node_t* create_tree()
{
	return (tree_node_t*)calloc(1,sizeof(tree_node_t));
}

node_t* alloc_node(void* data)
{
	node_t* new_node = (node_t*)calloc(1,sizeof(node_t));
	new_node->data = data;
	return new_node;
}
void free_node(node_t* node)
{
	free(node);
}

node_t* insert_node(node_t* root, endpoint_t* data)
{
	if(root == NULL)
		{root = alloc_node( (void*)data );return root;}
	if( data->fd > ((endpoint_t*)root->data)->fd )
		root->right = (void*)insert_node(root->right, data);
	else
		root->left = (void*)insert_node(root->left, data);
	return root;
}
void insert_tree(tree_node_t* tree, endpoint_t* data)
{
	if(tree->root == NULL)
		tree->root = alloc_node( (void*)data );
	else
		insert_node( tree->root, data );
}
void delete_tree_node(tree_node_t* tree, int fd)
{
	node_t* cur = tree->root;
	node_t* pre = cur;
	int flag;/*0 means left,1 means right*/
	if(((endpoint_t*)tree->root->data)->fd == fd) {
		if(tree->root->left == tree->root->right) {
			free((endpoint_t*)tree->root->data);free_node(tree->root);tree->root=NULL;return;
		}
	}
	else {
		while( ((endpoint_t*)cur->data)->fd != fd )
		{
			if( fd > ((endpoint_t*)cur->data)->fd ) {
				pre = cur;cur = cur->right;flag = 1;
			}
			else {
				pre = cur;cur = cur->left;flag = 0;
			}
		}
		free((endpoint_t*)cur->data);
	}
	while(cur != NULL) {
		if( cur->right != NULL)
			{flag = 1;pre = cur;cur->data = cur->right->data;cur = cur->right;}
		else if ( cur->right == NULL && cur->left != NULL)
			{flag = 0;pre = cur;cur->data = cur->left->data;cur = cur->left;}
		else 
			{free_node(cur);if(flag == 0) pre->left = NULL; else pre->right = NULL;cur=NULL;}
	}
}
void inorder_print_tree_node( node_t* root, int* buf , int* nfds)
{
	/* nfds should be zero init*/
	if( root != NULL )
	{
		inorder_print_tree_node( root->left, buf, nfds);
		buf[(*nfds)++] = ((endpoint_t*)root->data)->fd;
		inorder_print_tree_node( root->right, buf, nfds);
	}
}
