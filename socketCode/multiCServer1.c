/*
 * This is a simple server for multi-clients at a time.
 * We use sub-process by the function fork() to implement it.
 */

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define TPort 7800

int main(void)
{
	int server_sockfd, client_sockfd;
	unsigned int len;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	/* create a named listen socket for this sever */
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sockfd == -1)
	{
		perror("create server_sockfd error");
		exit(EXIT_FAILURE);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(TPort);
	if(bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind server_sockfd error");
		exit(EXIT_FAILURE);
	}

	/* listen to create the request queue */
	if(listen(server_sockfd, 5) == -1)
	{
		perror("listen server_sockfd error");
		exit(EXIT_FAILURE);
	}

	// to solve children's exit code: Just Ignore it.
	signal(SIGCHLD, SIG_IGN);

	/* loop to deal with all requests in the queue */
	while(1)
	{
		char ch;
		printf("server waiting\n");

		/* accept a connecting request and create a new socket for this client */
		len = sizeof(client_addr);//a **value-result** argrument must initailize
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &len);
		if(client_sockfd == -1)
		{
			perror("accept error");
			exit(EXIT_FAILURE);
		}

		/* fork a child process to work for this client */
		if(fork() == 0)
		{/* child for client */
			read(client_sockfd, &ch, 1);
			sleep(5); /* just for demonstration */
			++ch;
			write(client_sockfd, &ch, 1);
			close(client_sockfd);
			_exit(EXIT_SUCCESS);
		}
		/* parent: close the socket for client and continue to accept next requests */
		else
		{
			close(client_sockfd);
		}
	}

	return 0;
}
