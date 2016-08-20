/*
 * =====================================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  Header defines some data which are useful for both the client and the
 *    server.
 *    To implement a clients/server example base on named pipes, we use one fifo for the
 *    server and different fifos for the clients. And the data transfers between them must
 *    <= PIPE_BUF in blocked mode of pipes, in this case, clients and server would be blocked
 *    automatic by system.
 *    To return the data processed by the server to different clients, we should add extra
 *    information to identify the clients when transfer data to the server. So we transfer
 *    their PID along with data to the server and create the unique fifo for them. We can
 *    thansfer the processed data to the original client.
 *
 *        Created:  08/20/2016 05:11:02 PM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SERVER_FIFO_NAME "/tmp/serv_fifo"
#define CLIENT_FIFO_NAME "/tmp/cli_%d_fifo"

#define BUFFER_SIZE 20

struct data_to_pass_st
{
	pid_t client_pid;
	char data[BUFFER_SIZE + 1];
};
