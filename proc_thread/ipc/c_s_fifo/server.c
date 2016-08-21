/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  This server converts the data received to upper case. And transfers
 *    it back throngh certain fifo to the clients.
 *
 *        Created:  08/20/2016 09:38:54 PM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include "client.h"
#include <ctype.h>
#include <signal.h>


void clear(int sig)
{
	write(STDOUT_FILENO, "\nGot signal SIGINT", 18);
	write(STDOUT_FILENO, "\nClear the serverfifo", 22);
	if(access(SERVER_FIFO_NAME, F_OK) == 0)
		unlink(SERVER_FIFO_NAME);
	_exit(EXIT_FAILURE);
}

int main(void)
{
	signal(SIGINT, clear); /* when ctrl+c, user clear() to delete the server fifo */

	int server_fifo_fd, client_fifo_fd;
	int read_bytes;
	struct data_to_pass_st data;
	char *char_ptr;
	char client_fifo[256]; /* will store the client fifo name */

	/* create the server fifo and then open it in O_RDONLY mode */
	if(mkfifo(SERVER_FIFO_NAME, 0777) == -1)
	{
		perror("create server fifo failed");
		exit(EXIT_FAILURE);
	}
	server_fifo_fd = open(SERVER_FIFO_NAME, O_RDONLY);
	if(server_fifo_fd == -1)
	{
		perror("open server fifo failed");
		exit(EXIT_FAILURE);
	}

	open(SERVER_FIFO_NAME, O_WRONLY); /* make sure the read() will always blocked */

	/* lets client queue for demo purpose */
	sleep(2);

	/* read data from the opened fifo until its wirte-port was closed */
	do
	{
		read_bytes = read(server_fifo_fd, &data, sizeof(data));
		if(read_bytes > 0)
		{
			/* convert the received data to its uppercase */
			char_ptr = data.data;
			while(*char_ptr)
			{
				*char_ptr = toupper(*char_ptr);
				++char_ptr;
			}

			/* get the client fifo name with received pid */
			sprintf(client_fifo, CLIENT_FIFO_NAME, data.client_pid);

			/* open the client fifo in O_WRONLY mode and pass back the data processed */
			client_fifo_fd = open(client_fifo, O_WRONLY);
			if(client_fifo_fd == -1)
			{
				perror("open client fifo in O_WRONLY failed");
				exit(EXIT_FAILURE);
			}
			write(client_fifo_fd, &data, sizeof(data));

			/* close the client fifo */
			close(client_fifo_fd);
		}
	}
	while(read_bytes > 0);

	/* clear the server fifo */
	close(server_fifo_fd);
	unlink(SERVER_FIFO_NAME);

	return 0;
}
