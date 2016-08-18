/*
 * =====================================================================================
 *
 *       Filename:  fifo3_productor.c
 *
 *    Description:  To demonstrate IPC with named pipe, we need a productor which write
 *    to the pipe,and a consumer which read from the pipe.
 *    We use **blocked** open_mode to open the named pipe and write a data whose length is 
 *    **<=** the buffer of named pipe(PIPE_BUF). So that, we can avoid the mix of data writen
 *    by different processes.
 *    In this example, we just have one productor and one consumer. We do not care about 
 *    the content of data.
 *
 *        Created:  08/18/2016 02:18:19 PM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>

#define FIFO_NAME "/tmp/my_fifo"
#define TEN_MEG (1024 * 1024 * 10)

int main(void)
{
	int pipe_fd, res;
	int open_mode = O_WRONLY; /* blocked write only mode */
	int bytes_sent = 0;
	char buffer[PIPE_BUF + 1]; /* we don't care about the content, needn't initialization */

	/* if the name pipe is not exist, then create it */
	if(access(FIFO_NAME, F_OK) == -1)
	{
		res = mkfifo(FIFO_NAME, 0777);
		if(res == -1)
		{
			perror("mkfifo failed");
			exit(EXIT_FAILURE);
		}
	}

	/* open the pipe with O_WRONLY mode */
	printf("Process %d opening FIFO O_WRONLY\n", getpid());
	pipe_fd = open(FIFO_NAME, open_mode);
	printf("Process %d result %d\n", getpid(), pipe_fd);

	/* write 10M data to the pipe, and every time less-equal PIPE_BUF */
	if(pipe_fd != -1)
	{
		while(bytes_sent < TEN_MEG)
		{
			res = write(pipe_fd, buffer, PIPE_BUF);
			if(res == -1)
			{
				perror("wirte to pipe failed");
				exit(EXIT_FAILURE);
			}
			bytes_sent += res;
		}

		/* close the pipe_fd write port, so that the read() will return 0 
		 * instead of blocking to wait more data. */
		close(pipe_fd); /* close the pipe fd */
	}
	else
	{
		perror("open the fifo failed");
		exit(EXIT_FAILURE);
	}

	printf("Process %d finished\n", getpid());

	return 0;
}
