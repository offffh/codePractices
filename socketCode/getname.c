/*
 * A simple example to get a host's information. 
*/

#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *host, **names, **addrs;
	struct hostent *hostinfo;

	//get host name
	if(argc == 1)
	{
		char myname[256];
		gethostname(myname, 255);
		host = myname;
	}
	else
		host = argv[1];

	//get host information by name
	hostinfo = gethostbyname(host);
	if(!hostinfo)
	{
		fprintf(stderr, "can not get info for host: %s\n", host);
		exit(1);
	}

	//if succeed, display hostent
	printf("results for host %s:\n", host);
	printf("Name: %s\n", hostinfo->h_name);
	printf("Aliases:");
	names = hostinfo->h_aliases;
	while(*names)
	{
		printf(" %s", *names);
		++names;
	}
	printf("\n");

	//check whether the host is AF_INET
	if(hostinfo->h_addrtype != AF_INET)
	{
		fprintf(stderr, "not an IP host");
		exit(1);
	}
	//display all its IP addresses
	addrs = hostinfo->h_addr_list;
	while(*addrs)
	{
		printf(" %s", inet_ntoa(*(struct in_addr *)*addrs));
		++addrs;
	}
	printf("\n");

	return 0;
}
