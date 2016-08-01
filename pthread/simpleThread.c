/*
 * =====================================================================================
 *
 *       Filename:  simpleThread.c
 *
 *    Description:  This is the first multi-thread program: the thread modefied their
 *    global string msg, and exit with a string.
 *
 *        Created:  07/29/2016 12:20:33 PM
 *
 *       Compiler:	gcc -D_REENTRANT simpleThread.c -o simpleThread -lpthread
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void *thread_function(void *arg);
char msg[] = "Hello World!";

int main()
{
	int res; /* return value of some function */
	pthread_t a_thread; /* value to label a thread */
	void *thread_result; /* stores the exit value of a thread */

	char argMsg[] = "argMsg i love u";

	printf("msg is: %s\n", msg);

	/* create a thread which will start from thread_function and delivery msg to it */
	res = pthread_create(&a_thread, NULL, thread_function, argMsg);
	if(res != 0)
	{
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	/* wait for the thread's return (familiar with waitpid()) */
	printf("waiting for thread to finish...\n");
	res = pthread_join(a_thread, &thread_result);
	if(res != 0)
	{
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}

	/* if executed here, the thread was finished. */
	printf("Thread joined, it returned: %s\n", (char *)thread_result);
	printf("msg is now: %s\n", msg);

	return 0;
}

void *thread_function(void *arg)
{
	printf("thread_function is running. Argument was: %s\n", (char *)arg);
	sleep(3); /* just for demonstration */

	/* modify global variable */
	strcpy(msg, "Bye!");

	/* exit thread with a string */
	static char emsg[] = "Thank you for creating me";
	pthread_exit(emsg);
}
