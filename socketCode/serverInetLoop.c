/*
 * A simple server in a loopback network.
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
	int server_sockfd, client_sockfd;
	struct sockaddr_in server_address, client_address;
	unsigned int client_len;

	//create an unnamed socket
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sockfd == -1)
	{
		perror("create server_sockfd erro");
		exit(1);
	}

	//name this socket for this server
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(7800);
	if(bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		perror("bind server_address error");
		exit(1);
	}

	//listen() to create a queue for requests
	if(listen(server_sockfd, 5) == -1)
	{
		perror("listen server_sockfd error");
		exit(1);
	}

	//wait for requests for connecting
	while(1)
	{
		char ch;
		printf("server waiting\n");
		//accept a request
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
		if(client_sockfd == -1)
		{
			perror("accept client_sockfd error");
			exit(1);
		}
		//already connected, do some operations: 
		//read a character and plus it with one then reply it
		read(client_sockfd, &ch, 1);
		++ch;
		write(client_sockfd, &ch, 1);
		close(client_sockfd);
	}

	return 0;
}
