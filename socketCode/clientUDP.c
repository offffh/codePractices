/*
 * A simple example of client which would get a string form stdin and send a server for its
 * uppercase format.
 */

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BSIZE 255
#define UDP_TEST_PORT 7800
#define UDP_SERVER_IP "127.0.0.1"

int main(void)
{
	int sockfd, result;
	struct sockaddr_in saddr;
	char buffer[BSIZE];

	//create a udp socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1)
	{
		perror("creat client udp socket error");
		exit(EXIT_FAILURE);
	}

	//construct the server's sockaddr_in
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(UDP_SERVER_IP);
	saddr.sin_port = htons(UDP_TEST_PORT);
	
	//loop to query oprations
	while(1)
	{
		//get string from keyboard
		printf("Please enter a string(chars<=255) to send to server: \n");
		result = read(0, buffer, sizeof(buffer));

		//send string to server
		result = sendto(sockfd, buffer, result, 0, (struct sockaddr *)&saddr,
				sizeof(saddr));
		if(result == -1)
		{
			perror("client sendto error");
			exit(EXIT_FAILURE);
		}

		//get response string from server
		result = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
		if(result == -1)
		{
			perror("client recvfrom error");
			exit(EXIT_FAILURE);
		}
		
		//print the string
		buffer[result] = '\0';
		printf("Receive %d converted bytes:\n%s", result, buffer);
	}

	return 0;
}
