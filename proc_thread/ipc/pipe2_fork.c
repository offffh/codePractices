/*
 * =====================================================================================
 *
 *       Filename:  pipe2_fork.c
 *
 *    Description:  Pipe between the parent(write) and child(read) processes by fork().
 *
 *        Created:  08/09/2016 02:29:00 PM
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
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	int data_processed;
	int pipefd[2];
	char buffer[BUFSIZ + 1];
	const char data[] = "123456";

	pid_t pid_fork;

	memset(buffer, '\0', sizeof(buffer));

	if(pipe(pipefd) == -1)
	{
		perror("create pipe failed");
		exit(EXIT_FAILURE);
	}

	pid_fork = fork();
	switch(pid_fork)
	{
		case -1:
			perror("fork failed");
			exit(EXIT_FAILURE);
		case 0: /* child, read data*/
			data_processed = read(pipefd[0], buffer, BUFSIZ);
			printf("Read %d bytes: %s\n", data_processed, buffer);
			exit(EXIT_SUCCESS);
		default: /* parent, write data */
			data_processed = write(pipefd[1], data, strlen(data));
			printf("Wrote %d bytes\n", data_processed);
			wait(NULL);
			exit(EXIT_SUCCESS);
	}

	return 0;
}

