#include "../base/hash.h"

int main()
{
	hash_slot_t** test_hash_table = hash_init();
	hash_insert( test_hash_table, "siwo", NULL );
	hash_delete( test_hash_table, "siwo");
	hash_insert( test_hash_table, "siwo", NULL );
	hash_slot_t* hs = hash_find( test_hash_table, "siwo");
	printf("%s\n",hs->name);
	hash_delete( test_hash_table, "siwo");
	return 0;
}
