/*
 * A simple example about how to exec* function
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *const ps_argv[] = { "ps", "lf", 0 };
	printf("Running ps with execlp()\n");
	//replace the process image, so after execvp(), pre-process will not be executed continue.
	if(execvp("ps", ps_argv) == -1)
	{
		perror("execvp error");
		exit(EXIT_FAILURE);
	}
	//so if execvp sucess, it will not print 'Done'.
	printf("Done.\n");

	exit(EXIT_SUCCESS);
}
