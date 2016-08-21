/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  This client sends its pid and data to the server throng the O_WRONLY
 *    server fifo. And open its own client fifo identified by pid in O_RDONLY mode to wait
 *    for data processed.
 *    This clinet will send 5 times data just for demonstration.
 *
 *        Created:  08/21/2016 03:59:47 PM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include "client.h"

int main(void)
{
	int server_fifo_fd, client_fifo_fd;
	struct data_to_pass_st data;
	int times = 5;
	char client_fifo[256];

	/* check the existence of the server fifo and then open it O_WRONLY */
	if(access(SERVER_FIFO_NAME, F_OK) == -1)
	{
		perror("the server fifo isn't exist");
		exit(EXIT_FAILURE);
	}
	else
		server_fifo_fd = open(SERVER_FIFO_NAME, O_WRONLY);
	
	/* create client fifo with pid information */
	sprintf(client_fifo, CLIENT_FIFO_NAME, getpid());
	if(mkfifo(client_fifo, 0777) == -1)
	{
		perror("create client fifo failed");
		exit(EXIT_FAILURE);
	}

	/* send and get for 5 times' demo */
	while(times--)
	{
		/* prepare the data contents */
		data.client_pid = getpid();
		sprintf(data.data, "*Data from %d*", data.client_pid);

		printf("%d sent %s, ", data.client_pid, data.data);

		/* send data and pid to the server fifo */
		write(server_fifo_fd, &data, sizeof(data));

		/* open client fifo in O_RDONLY mode */
		client_fifo_fd = open(client_fifo, O_RDONLY);
		if(client_fifo_fd == -1)
		{
			perror("open client fifo in O_RDONLY failed");
			exit(EXIT_FAILURE);
		}

		/* read data from the client fifo until returns 0 */
		if(read(client_fifo_fd, &data, sizeof(data)) > 0)
			printf("received: %s\n", data.data);

		/* close client fifo */
		close(client_fifo_fd);
	}

	/* close the server fifo and clear the client fifo */
	close(server_fifo_fd);
	unlink(client_fifo);

	return 0;
}
