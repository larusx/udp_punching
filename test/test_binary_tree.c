#include "../base/binary_tree.h"
void inorder_print(node_t* root)
{
	if(root != NULL)
	{
		inorder_print(root->left);
		printf("%d ",((endpoint_t*)root->data)->fd);
		inorder_print(root->right);
	}
}
int main()
{
	int fd[]={6,4,8,3,5,7,10};
	tree_node_t* tree = create_tree();
	endpoint_t* ep;
	for(int i=0; i<7; i++)
	{
		ep = (endpoint_t*)malloc(sizeof(endpoint_t)); 
		ep->fd = fd[i];
		insert_tree(tree, ep);
	}
	inorder_print(tree->root);
	printf("\n");
	delete_tree_node(tree, 4);
	delete_tree_node(tree, 6);
	delete_tree_node(tree, 10);
	delete_tree_node(tree, 8);
	delete_tree_node(tree, 3);
	delete_tree_node(tree, 5);
	delete_tree_node(tree, 7);
	inorder_print(tree->root);
	free(tree);
	printf("\n");
}
