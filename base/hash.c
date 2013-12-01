#include "hash.h"

/*
 * compute hash key from str
 */
int hash_key( char* str, int len )
{
	int key = 0;
	for ( int i = 0; i != len; i++) {
		key = key*31 + str[i];	
	}
	return key;
}
/*
 * init hash buckets
 */
hash_slot_t** hash_init(/* int n */)
{
	hash_slot_t** buckets = (hash_slot_t**)calloc( HASHSIZE, sizeof(hash_slot_t*) );
	return buckets;
}
/*
 * add slot to buckets
 */
void hash_insert( hash_slot_t** table, char* name, int len, endpoint_t* ep )
{
	hash_slot_t* node = (hash_slot_t*)malloc( sizeof(hash_slot_t) );	
	strncpy( node->name, name, len );
	node->len = len;
	node->ep = ep;
	node->next = NULL;
	int key = hash_key( name, len ) % HASHSIZE;
	if( table[key] == NULL )
		{table[key] = node;return;}
	hash_slot_t* col = table[key];
	while( col->next != NULL )
		col = col->next;
	col->next = node;
	return;
}
/*
 * find slot in buckets
 */
hash_slot_t* hash_find( hash_slot_t** table, char* name, int len )
{
	int key = hash_key( name, len ) % HASHSIZE;
	hash_slot_t* col = table[key];
	while( col != NULL) {
		if ( strncmp( col->name, name, len ) == 0)
			return col;
		col = col->next;
	}	
	return NULL;
}
/*
 * delete slot in buckets
 */
void hash_delete( hash_slot_t** table, char* name, int len )
{
	int key = hash_key( name, len ) % HASHSIZE;
	hash_slot_t* col = table[key];
	if ( strncmp( col->name, name, len ) == 0 ) {
	  table[key] = col->next;free(col->ep);free(col);return;	
	}
	hash_slot_t* tmp = col;
	while( strncmp( col->next->name, name, len ) ) 
		col = col->next;
	tmp = col->next;
	col->next = tmp->next;
	free(tmp->ep);
	free(tmp);
}
/*
 * iter hash_table return  
 */
int hash_iter( hash_slot_t** table, char* buf )
{
	int nbytes = 0;
	hash_slot_t* iter;
	for ( int i = 0; i < HASHSIZE; i++ ) {
		iter = table[i];
		while( iter != NULL ) {
			memcpy( &buf[nbytes], iter->name, iter->len );
			nbytes += iter->len;
			buf[nbytes++] = ' ';
			iter = iter->next;
		}
	}
	buf[nbytes] = '\n';
	return nbytes+1;
}
