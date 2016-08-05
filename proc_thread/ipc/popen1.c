/*
 * =====================================================================================
 *
 *       Filename:  popen1.c
 *
 *    Description:  Using an unnamed pipe to read the output of the command "uname -a" 
 *    which could print system information.
 *
 *        Created:  08/02/2016 07:26:57 PM
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
	FILE *read_fp;
	char buffer[BUFSIZ + 1];
	int chars_read;

	memset(buffer, '\0', sizeof(buffer)); /* initialize */

	read_fp = popen("uname -a", "r"); /* open read-pipe file */
	if(read_fp != NULL)
	{
		/* read data from the pipe file */
		chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
		if(chars_read > 0)
			printf("Output was %d chars:-\n%s\n", chars_read, buffer);
		exit(EXIT_SUCCESS);
	}
	perror("popen command 'uname -a'");
	exit(EXIT_FAILURE);

	return 0;
}
