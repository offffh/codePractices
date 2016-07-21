/* get a date time from the daytime service in xinetd by UDP */
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
	char buffer[128];
	const char *host;
	int sockfd, result;
	struct sockaddr_in addr;
	struct hostent *hostinfo;
	struct servent *servinfo;

	//get the host name at first
	if(argc == 1)
		host = "localhost";
	else
		host = argv[1];

	//get the host address
	hostinfo = gethostbyname(host);
	if(!hostinfo)
	{
		perror("get host info error");
		exit(EXIT_FAILURE);
	}

	//check that the daytime service exists on the host
	servinfo = getservbyname("daytime", "udp");
	if(!servinfo)
	{
		perror("get server info error");
		exit(EXIT_FAILURE);
	}
	printf("daytime port is %d\n", ntohs(servinfo->s_port));

	//create an udp socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1)
	{
		perror("create udp socket error");
		exit(EXIT_FAILURE);
	}

	//construct the address to query time
	addr.sin_family = AF_INET;
	addr.sin_port = servinfo->s_port;
	addr.sin_addr = *(struct in_addr *)*hostinfo->h_addr_list;

	//query time from the host
	result = sendto(sockfd, buffer, 1, 0, (struct sockaddr *)&addr,
			sizeof(addr));
	result = recvfrom(sockfd, buffer, sizeof(buffer), 0,
			NULL, NULL);

	buffer[result] = 0;

	printf("read %d bytes: %s", result, buffer);

	//close the socket
	close(sockfd);
	
	return 0;
}
