/*
 * A simple example which is a server to convert input string to its uppercase
 * and reply in datagram(UDP).
*/

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define BSIZE 255
#define UDP_TEST_PORT 7800

//convert string to uppercase
void stoupper(char *str)
{
	int i, len = strlen(str);
	for(i = 0; i < len; ++i)
		str[i] = toupper(str[i]);
}

int main(void)
{
	char buffer[BSIZE];
	struct sockaddr_in addr, fromaddr;
	int sockfd, result;
	unsigned int len = sizeof(struct sockaddr_in);

	//create an udp socket and name it
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1)
	{
		perror("create udp socket error");
		exit(EXIT_FAILURE);
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(UDP_TEST_PORT);
	if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind socket error");
		exit(EXIT_FAILURE);
	}

	//work for requests
	while(1)
	{
		//receive a message from the client socket
		result = recvfrom(sockfd, buffer, sizeof(buffer), 0,
				(struct sockaddr *)&fromaddr, &len);//len must be initialize
		if(result == -1)
		{
			perror("server receive error");
			exit(EXIT_FAILURE);
		}
		buffer[result] = '\0';

		printf("Received a string from client %s, string is: %s\n",
				inet_ntoa(fromaddr.sin_addr), buffer);
		
		//convert the input string to its uppercase format
		stoupper(buffer);
		printf("Convert to uppercase...\n");

		//response to the client
		result = sendto(sockfd, buffer, result, 0, (struct sockaddr *)&fromaddr, len);
		if(result == -1)
		{
			perror("server sendto error");
			exit(EXIT_FAILURE);
		}
	}

	//close the opened socket
	close(sockfd);


	return 0;
}
