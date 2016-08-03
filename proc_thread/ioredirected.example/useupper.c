/*
 * Use the upper.c program to convert a input file to its uppercase.
 * We use freopen to redirect the stdio and execl to use the upper.c program.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *filename;
	if(argc != 2)
	{
		/* stderr has no buffer and could immediately output the error msg */
		fprintf(stderr, "usage: useupper file\n");
		exit(1);
	}

	filename = argv[1];
	
	//redirect the standard input to the argv[1] file
	if(!freopen(filename, "r", stdin))
	{
		fprintf(stderr, "could not redirect stdin from file %s\n", filename);
		exit(2);
	}
	//use execl to call the upper pragram
	if(execl("./upper", "upper", NULL) == -1)
	{
		perror("could not exec ./upper");
		exit(3);
	}
	return 0;
}
