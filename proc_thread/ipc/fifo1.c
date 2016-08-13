/*
 * =====================================================================================
 *
 *       Filename:  fifo1.c
 *
 *    Description:  Just create a named fifo.
 *
 *        Created:  08/11/2016 05:53:38 PM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
	mode_t mask;
	struct stat sb;
	const char *fifoName = "/tmp/my_fifo";

	/* before creation, test the existence of the fifo file */
	if(access(fifoName, F_OK) == 0) /* exist already and then remove it */
		if(remove(fifoName) == -1)
		{
			perror("remove the old fifo file");
			exit(EXIT_FAILURE);
		}

	/* create a pipe named 'my_fifo' and its permissions are  requested to 'rwx-rwx-rwx' */
	int res = mkfifo(fifoName, 0777);
	printf("I want to create a named pipe: /tmp/my_fifo, permission want to be: %04o\n", 0777);

	/* get current umask without changing it */
	mask = umask(0); /* get current user mask */
	umask(mask); /* reset the user mask */
	printf("umask:%04o\n", mask); 

	if(res == -1)
	{
		perror("create named pipe failed");
		exit(EXIT_FAILURE);
	}
	printf("FIFO created.\n");

	/* get fifo file status */
	if(stat(fifoName, &sb) == -1)
	{
		perror("stat");
		exit(EXIT_FAILURE);
	}
	printf("fifo permission now: %04o\n", sb.st_mode & 000777);

	return 0;
}
