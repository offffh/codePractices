/*
 * =====================================================================================
 *
 *       Filename:  pipe1.c
 *
 *    Description:  a very simple example of pipe() usage in one process.
 *
 *        Created:  08/08/2016 03:43:39 PM
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

int main(void)
{
	int pipefd[2];
	const char data[] = "123456";
	char buffer[BUFSIZ + 1];
	int data_processed;

	memset(buffer, '\0', sizeof(buffer));

	if(pipe(pipefd) == 0)
	{
		data_processed = write(pipefd[1], data, strlen(data));
		printf("Wrote %d bytes\n", data_processed);
		data_processed = read(pipefd[0], buffer, BUFSIZ);
		printf("Read %d bytes: %s\n", data_processed, buffer);
		exit(EXIT_SUCCESS);
	}

	exit(EXIT_FAILURE);
}
