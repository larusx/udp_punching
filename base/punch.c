#include "punch.h"

endpoint_t* get_tcp_endpoint( const char* ip, int port )
{
	endpoint_t* ep = ( endpoint_t* )malloc( sizeof( endpoint_t ) );
	memset( &ep->addr, 0, sockaddrlen );
	if ( port != 0)
		ep->fd = socket( AF_INET, SOCK_STREAM, 0 );
	ep->addr.sin_family = AF_INET;
	if ( port != 0)
		ep->addr.sin_port = htons( port );
	if ( ip != NULL )
		ep->addr.sin_addr.s_addr = inet_addr( ip ); 
	else
		ep->addr.sin_addr.s_addr = INADDR_ANY;
	return ep;
}
