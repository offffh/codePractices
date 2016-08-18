/*
 * =====================================================================================
 *
 *       Filename:  fifo4_consumer.c
 *
 *    Description:  A consumer which read data from a named pipe. Just demonstration of 
 *    IPC with named pipe along with fifo3_productor.c.
 *
 *        Created:  08/18/2016 19:15:30 PM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#define FIFO_NAME "/tmp/my_fifo"

int main(void)
{
	int pipe_fd, res;
	int open_mode = O_RDONLY;
	int bytes_read = 0;
	char buffer[PIPE_BUF + 1];

	memset(buffer, '\0', sizeof(buffer));

	/* open the named pipe in O_RDONLY mode */
	printf("Process %d opening FIFO O_RDONLY\n", getpid());
	pipe_fd = open(FIFO_NAME, open_mode);
	printf("Process %d result %d\n", getpid(), pipe_fd);

	if(pipe_fd == -1)
	{
		perror("open fifo failed");
		exit(EXIT_FAILURE);
	}

	/* read data from the pipe until it return 0, and reckon up bytes recieved */
	do
	{
		res = read(pipe_fd, buffer, PIPE_BUF);
		bytes_read += res;
	}
	while(res > 0);

	close(pipe_fd); /* close the read port of pipe */

	printf("Process %d finished, %d bytes read\n", getpid(), bytes_read);

	return 0;
}
