/*
 * =====================================================================================
 *
 *       Filename:  pipe3_productor.c
 *
 *    Description:  This is a data productor. It would create a pipe and invoke the child process which would be another program to be the data consumer.
 *
 *        Created:  08/09/2016 06:21:03 PM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	int pipefd[2];
	int data_processed;
	const char data[] = "123456";
	char buffer[BUFSIZ + 1];
	pid_t pid_fork;

	memset(buffer, '\0', sizeof(buffer));

	/* create pipe */
	if(pipe(pipefd) == -1)
	{
		perror("create pipe failed");
		exit(EXIT_FAILURE);
	}

	pid_fork = fork();
	if(pid_fork == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}

	if(pid_fork == 0) /* child to be executed */
	{
		sprintf(buffer, "%d", pipefd[0]); /* convert pipefd[0] to a string */
		if(execl("pipe4_consumer", "pipe4_consumer", buffer, NULL) == -1)
		{
			perror("execute file failed");
			exit(EXIT_FAILURE);
		}
	}
	else /* parent */
	{
		data_processed = write(pipefd[1], data, strlen(data));
		printf("%d - wrote %d bytes\n", getpid(), data_processed);
	}

	return 0;
}

