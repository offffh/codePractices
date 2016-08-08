/*
 * =====================================================================================
 *
 *       Filename:  popen3.c
 *
 *    Description:  Read lots of data through the pipe.
 *    Using an unnamed pipe to read the output of the command "ps x".
 *
 *        Created:  08/05/2016 10:34:39 AM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	FILE *read_fp;
	char buffer[BUFSIZ + 1];
	int chars;

	/* initialize buffer with '\0' */
	memset(buffer, '\0', sizeof(buffer));
	/* pipe stream from a process */
	read_fp = popen("ps x", "r");
	if(read_fp == NULL)
	{
		perror("popen 'ps x' failed");
		exit(EXIT_FAILURE);
	}

	/* read the opened pipe stream in a loop */
	chars = fread(buffer, sizeof(char), BUFSIZ, read_fp);
	while(chars > 0)
	{
		buffer[chars - 1] = '\0';
		printf("Reading %d:-\n %s\n", BUFSIZ, buffer);
		chars = fread(buffer, sizeof(char), BUFSIZ, read_fp);
	}
	/* close the pipe */
	pclose(read_fp);

	exit(EXIT_SUCCESS);

}
