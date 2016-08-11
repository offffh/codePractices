/*
 * =====================================================================================
 *
 *       Filename:  pipe5_dup.c
 *
 *    Description:  Redirect pipe-fd as stdin or stdout by dup() or dup2().
 *    So we could call the standard programs which need none of file descriptions.
 *    The parent process would write data into the pipe and the child process replaces its 'stdin' by the pipefd[0]. So the child would execute another process which read from standard-in.
 *
 *        Created:  08/11/2016 03:10:54 PM
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
	pid_t pid_fork;

	/* create a pipe */
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
	else if(pid_fork == 0) /* child */
	{
		close(0); /* close child's stdin */
		dup(pipefd[0]); /* duplicate pipefd[0] to 0 */

		/* clear unused file descriptor */
		close(pipefd[0]);
		close(pipefd[1]);

		/* execute another process */
		execlp("od", "od", "-c", NULL);
		exit(EXIT_FAILURE);
	}
	else /* parent */
	{
		close(pipefd[0]);
		data_processed = write(pipefd[1], data, strlen(data));
		close(pipefd[1]); /* close input pipe for child to detect the end-of-file */
		printf("%d - wrote %d bytes\n", getpid(), data_processed);
	}

	return 0;
}
