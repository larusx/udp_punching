#include "../base/base.h"
#include "../base/punch.h"

int main()
{
	int on=1;
	endpoint_t* remote_endpoint = get_endpoint( "127.0.0.1", 80 ,UDP_ENDPOINT );
	char buf[1024] = "udp send test\n";
	socklen_t addr_len = sockaddrlen;
	sendto( remote_endpoint->fd, buf, 1024, 0 ,(struct sockaddr*)&remote_endpoint->addr, sockaddrlen);
	perror("sendto");
	int nbytes = recvfrom( remote_endpoint->fd, buf, 1024, 0 ,(struct sockaddr*)&remote_endpoint->addr, &addr_len);
	buf[nbytes] = 0;
	printf("%s",buf);
	return 0;
}
