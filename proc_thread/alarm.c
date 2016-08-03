/*
 * =====================================================================================
 *
 *       Filename:  alarm.c
 *
 *    Description:  Simulate the alarm(). The child process would send a SIGALRM signal
 *    to its parent after waiting for 5 seconds, and printf "Ding\n".
 *
 *        Created:  07/28/2016 01:36:25 AM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void ding(int sig)
{
	write(STDIN_FILENO, "Ding\n", 5);
}

int main(void)
{
	printf("alarm application starting\n");

	pid_t pid = fork();
	switch(pid)
	{
		case -1:
			/* fail to fork a child */
			perror("fork error");
			exit(EXIT_FAILURE);
		case 0:
			/* child */
			sleep(5);
			/* send signal to parent */
			kill(getppid(), SIGALRM);
			_exit(EXIT_SUCCESS);
	}

	/* parent */
	printf("waiting for alarm to go off\n");
	printf("<5 second pause>\n");
	signal(SIGALRM, ding); /* catch the signal */

	pause(); /* pause the parent until the signal */

	return 0;
}
