#ifndef _BINARY_TREE
#define _BINARY_TREE
#include <stdlib.h>
typedef struct node node_t;
struct node{
	struct  data;   //FIXME
	node_t* left;
	node_t* right;
};

struct tree_node{
	node_t* root;
};

tree_node* create_tree()
{
	return (tree_node*)calloc(1,sizeof(tree_node));
}

node_t* alloc_node(int data)
{
	node_t* new_node = (node_t*)calloc(1,sizeof(node_t));
	new_node->data = data;
	return new_node;
}
node_t* insert_node(node_t* root, int data)
{
	if(root == NULL)
		{root = alloc_node(data);return root;}
	if(data > root->data)
		root->right = insert_tree(root->right, data);
	else
		root->left = insert_tree(root->left, data);
	return root;
}
void delete_node(node_t* root, int data)
{

}
#endif
