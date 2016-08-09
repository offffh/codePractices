/*
 * =====================================================================================
 *
 *       Filename:  pipe4_consumer.c
 *
 *    Description:  A data consumer to be executed by pipe3_producotr.c with execl().
 *
 *        Created:  08/09/2016 06:46:56 PM
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

int main(int argc, char **argv)
{
	int data_processed;
	char buffer[BUFSIZ +1];
	int fd;

	memset(buffer, '\0', sizeof(buffer));

	sscanf(argv[1], "%d", &fd); /* convert string to int file description */

	data_processed = read(fd, buffer, BUFSIZ);
	printf("%d - read %d bytes: %s\n", getpid(), data_processed, buffer);

	return 0;
}

