/*
 * An example of fork() and wait().
 * The father process will complete the print work earlierly than its child, 
 * so the father must wait the child!
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	const char *msg;
	int n;
	int exit_code;

	printf("fork program staring\n");
	pid = fork();

	switch(pid)
	{
		case -1:
			perror("fork error");
			exit(EXIT_FAILURE);
		case 0:
			msg = "This is the child";
			n = 5;
			exit_code = 37;
			break;
		default:
			msg = "This is the parent";
			n = 3;
			exit_code = 0;
			break;
	}

	while(n--)
	{
		puts(msg);
		sleep(1);
	}
	
	//parent process to wait for child
	if(pid != 0)
	{
		int stat_val;
		pid_t child_pid;

		child_pid = wait(&stat_val);
		
		printf("Child has finished: PID = %d\n", child_pid);
		if(WIFEXITED(stat_val))
			printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
		else
			printf("Child terminated abnormally\n");
	}

	exit(exit_code);
}
