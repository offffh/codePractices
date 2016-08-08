/*
 * =====================================================================================
 *
 *       Filename:  popen4.c
 *
 *    Description:  Demonstrate that popen() would open the command"cat popen*.c | wc -l" by invoking the shell.
 *
 *        Created:  08/08/2016 11:14:08 AM
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
	int read_chars;
	
	memset(buffer, '\0', sizeof(buffer));
	read_fp = popen("cat popen*c | wc -l", "r");
	if(read_fp == NULL)
	{
		perror("popen process failde");
		exit(EXIT_FAILURE);
	}

	read_chars = fread(buffer, sizeof(char), BUFSIZ, read_fp);
	while(read_chars > 0)
	{
		buffer[read_chars - 1] = '\0';
		printf("Reading:-\n %s\n", buffer);
		read_chars = fread(buffer, sizeof(char), BUFSIZ, read_fp);
	}
	pclose(read_fp);

	return 0;
}
