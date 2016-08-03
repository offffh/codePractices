/*
 * =====================================================================================
 *
 *       Filename:  ctrlcSA.c
 *
 *    Description:  Just like ctrlc.c. Except of the more reliable and secure version of
 *    signal: sigaction.
 *
 *        Created:  07/28/2016 01:53:11 AM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

void ouch(int sig)
{
	const char msg[] = "\nOUCH! - I got signal ";
	char char_sig = '0' + sig;
	/* system call write is secure and reentrant */
	write(STDIN_FILENO, msg, sizeof(msg) - 1);
	write(STDIN_FILENO, &char_sig, 1);
	write(STDIN_FILENO, "\n", 1);
}

int main(void)
{
	/* initialize the signal action */
	struct sigaction act;
	act.sa_handler = ouch; /* signal handle function */
	act.sa_flags = SA_RESETHAND; /* reset signal's handle just like SIG_DFL */
	sigemptyset(&act.sa_mask); /* don't block any signals */

	/* set to catch-handling SIGINT */
	sigaction(SIGINT, &act, 0);

	while(1)
	{
		printf("Hello world\n");
		sleep(1);
	}

	return 0;
}

