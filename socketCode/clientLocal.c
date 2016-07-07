#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	int sockfd;
	struct sockaddr_un address;
	char ch = 'A';

	//creat a unnamed socket for client
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("creat sockfd for client error");
		exit(1);
	}

	//set the server address
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "server_socket");

	//connect the server by our socket to its address
	if(connect(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		perror("connect error");
		exit(1);
	}

	//do some operations
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("char from server = %c\n", ch);

	return 0;
}
