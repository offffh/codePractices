/*
 * =====================================================================================
 *
 *       Filename:  fifo2.c
 *
 *    Description:  Open the named pipe with diferent 'mode_t' parameters:
 *    O_RDONLY, O_WRONLY, O_RDONLY | O_NONBLOCK, O_WRONLY | O_NONBLOCK.
 *    Just for demonstration.
 *
 *        Created:  08/15/2016 01:29:18 PM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_NAME "/tmp/my_fifo"

int main(int argc, char **argv)
{
	int res, i;
	int open_mode = 0;

	/* check the arguments */
	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s <some combination of O_RDONLY O_WRONLY O_NONBLOCK>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* get open_mode value from the comand line arguments */
	for(i = 1; i < argc; ++i)
	{
		if(strncmp(*++argv, "O_RDONLY", 8) == 0)
			open_mode |= O_RDONLY;
		if(strncmp(*argv, "O_WRONLY", 8) == 0)
			open_mode |= O_WRONLY;
		if(strncmp(*argv, "O_NONBLOCK", 10) == 0)
			open_mode |= O_NONBLOCK;
	}

	/* if there are none pipe files, then create one named pipe */
	if(access(FIFO_NAME, F_OK) == -1)
	{
		res = mkfifo(FIFO_NAME, 0777);
		if(res == -1)
		{
			perror("mkfifo failed");
			exit(EXIT_FAILURE);
		}
	}

	/* open the named pipe */
	printf("Process %d opening FIFO\n", getpid());
	res = open(FIFO_NAME, open_mode);
	printf("Process %d result %d\n", getpid(), res);
	sleep(5);
	if(res != -1)
		close(res);
	printf("Process %d finished\n", getpid());

	return 0;
}
