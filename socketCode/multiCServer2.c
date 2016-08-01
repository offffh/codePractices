/*
 * =====================================================================================
 *
 *       Filename:  multiCServer2.c
 *
 *    Description:  Implement a server for multi-clients using select().
 *
 *        Created:  07/29/2016 12:16:25 PM
 *       Compiler:  gcc
 *
 *         Author:  offffh wuzhiyang87@gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
