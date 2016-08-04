/*
 * =====================================================================================
 *
 *       Filename:  popen2.c
 *
 *    Description:  Using popen to write to a pipe-file of command 'od -c'.Just a simple
 *    example of popen(command, "w").
 *
 *        Created:  08/04/2016 05:05:20 PM
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
	FILE *write_fp;
	char buffer[BUFSIZ +1];

	/* output s string into buffer */
	sprintf(buffer, "Once upon a time, there was ...\n");
	write_fp = popen("od -c", "w"); /* open a write-pipe file */

	if(write_fp == NULL)
	{
		perror("popen command 'od -c' error");
		exit(EXIT_FAILURE);
	}

	/* write to the pipe file */
	fwrite(buffer, sizeof(char), strlen(buffer), write_fp);
	
	pclose(write_fp);
	
	exit(EXIT_SUCCESS);
}
