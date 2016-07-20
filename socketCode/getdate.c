/*
 * Connect to a system server named 'getdata' to print system time.
 * In any host!
 */
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char **argv)
{
	int sockfd, result;
	const char *host;
	struct hostent *hostinfo;
	struct servent *servinfo;
	struct sockaddr_in addr;
	char buffer[128];//to store the daytime output
	
	//given a host name to query
	if(argc == 1)
		host = "localhost";
	else
		host = argv[1];
	
	//get the address of the host
	hostinfo = gethostbyname(host);
	if(!hostinfo)
	{
		fprintf(stderr, "no host: %s", host);
		exit(1);
	}
	//print hostinfo
	printf("host %s:\n", host);
	printf("ip: %s\n", inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list));

	//check whether it has a daytime sever on the host
	servinfo = getservbyname("daytime", "tcp");
	if(!servinfo)
	{
		fprintf(stderr, "no daytime service\n");
		exit(1);
	}
	printf("daytime port is %d\n", ntohs(servinfo->s_port));

	//if exacute to here, we are prepared to connect the server
	//create an unnamed socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		fprintf(stderr, "creat socket failed!\n");
		exit(1);
	}
	//set the server address for connecting
	addr.sin_family = AF_INET;
	addr.sin_port = servinfo->s_port;
	addr.sin_addr = *(struct in_addr *)*hostinfo->h_addr_list;

	//connect and get date and time
	if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("connect error!");
		exit(1);
	}

	result = read(sockfd, buffer, sizeof(buffer));
	buffer[result] = '\0';
	printf("read %d bytes: %s", result, buffer);

	//disconncet the socket
	close(sockfd);

	return 0;
}
