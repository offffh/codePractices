#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#define Try__exit

int glob = 6;
int main()
{
	int var = 88;
	FILE *out = fopen("b.txt", "w");
	pid_t pid;
	fprintf(out, "a");/* 'a' will be put in the file-stream-buffer in memory.
						 And fork will copy this buffer */
	printf("before vfork: glob = %d\tvar = %d\n", glob, var);
	if((pid = fork()) == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0) /* child process */
	{
		fprintf(out, "b");
		++glob;
		++var;
#ifdef Try__exit
		_exit(0);/* b.txt will be "ac" with _exit().
				It closes the file pointed by out without flushing its file-stream-buffer "ab". */
#else
		exit(0);/* b.txt will be "acbc" with exit().
				It flush its file-stream-buffer "ab" before closing the file. */
#endif
	}

	/* parent process */
	fprintf(out, "c");
	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	exit(0);

}
