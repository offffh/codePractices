#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
	int server_sockfd, client_sockfd;
	int client_len;
	struct sockaddr_un server_address, client_address;

	//remove the old socket
	unlink("server_socket");
	//creat an unnamed socket for this server at first
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sever_sockfd == -1)
	{
		perror("creat server_sockfd error");
		exit(0);
	}
	//name this socket by the address of this server
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	if(bind(server_sockfd,(struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		perror("bind server_sockfd error");
		exit(0);
	}
	//listen() to creat a queue for requests
	if(listen(server_sockfd, 5) == -1)
	{
		perror("listen server_sockfd error");
		exit(0);
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
			exit(0);
		}
		//already connected, do some operations
	}
}
