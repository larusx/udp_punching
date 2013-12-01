#ifndef _HASH_H
#define _HASH_H
#include "punch.h"

#define HASHSIZE 127

typedef struct hash_slot hash_slot_t;
struct hash_slot{
	char name[10];
	int len;
	endpoint_t* ep;
	struct hash_slot* next;
};

int hash_key( char* str, int len );/*compute hash key from string*/
/*
 * hash function supported
 */
hash_slot_t** hash_init( void );

void hash_insert( hash_slot_t** table, char* name, int len, endpoint_t* ep );

hash_slot_t* hash_find( hash_slot_t** table, char* name, int len );

void hash_delete( hash_slot_t** table, char* name, int len );

int hash_iter( hash_slot_t** table, char* buf );
#endif
