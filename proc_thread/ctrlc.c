/*
 * =====================================================================================
 *
 *       Filename:  ctrlc.c
 *
 *    Description:  A simple example to introduce u how to deal with catched signals by
 *    signal(): At your first press on ctrl-c, the process just print some message. And
 *    the second time of ctrl-c will indeed interrupt the process.
 *    ^-^:signal has a more reliable version now named sigaction().
 *
 *        Created:  07/27/2016 01:54:32 AM
 *       Compiler:  gcc
 *
 *         Author:  offffh 
 *
 * =====================================================================================
 */
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ouch(int sig)
{
	printf("OUCH! - I got signal %d\n", sig);
	//Default handling the sig named SIGINT
	(void) signal(SIGINT, SIG_DFL);
}

int main()
{
	//handling sig SIGINT with ouch function
	(void) signal(SIGINT, ouch);

	while(1)
	{
		printf("Hello World!\n");
		sleep(1);
	}
	return 0;
}
