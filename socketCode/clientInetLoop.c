/*
 * a simple client in loopback network
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main(void)
{
	int sockfd;
	struct sockaddr_in address;
	char ch = 'A';

	//create an unnamed socket for client
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("create sockfd for client error");
		exit(1);
	}

	//set the server's address
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(7800);

	//request to connect the server
	if(connect(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		perror("connet sockfd error");
		exit(1);
	}

	//do some operations
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("char from server = %c\n", ch);

	return 0;
}
