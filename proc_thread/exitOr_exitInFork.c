/*
 * An example demonstrates the difference between exit and _exit in parent-children processes.
 * (Implemention is)Different from their introduction, the exit and _exit both don't close the files the processed shared immediately.Instead, the kernel will close the files which will not be used by any process any more.
 * ^-^: A file are shared between processes and they just have different file pointers(FILE *) which point to the same file. We can consider that the file pointers are similar with the smart pointer shared_ptr of C++ language. That means every _exit or exit just destroy its file pointers and decrease the file's ref-count. When the ref-count is zero aka there is no file pointer pointing to the file, the file will be closed at now.
 */

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
	fprintf(out, "c");/* the file pointer 'out' could always be manipulated after _exit or exit above.
						 Because, it's unnessary to close the file immediately when _exit and exit, 
						 the linux kernel would close them when the process's termination. */
	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	exit(0);

}
