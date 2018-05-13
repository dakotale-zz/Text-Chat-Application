/*
** client.c -- a stream socket client demo
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include "../include/global.h"
#include "../include/logger.h"

#define PORT 3490 // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	return &(((struct sockaddr_in*)sa)->sin_addr);
}

int createSocket_client()
{
	int sockfd = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		cse4589_print_and_log("Can't open socket");

	cse4589_print_and_log("Create function on server called");
	cse4589_print_and_log("\n");

	return sockfd;
}

void bind_client()
{
	struct sockaddr_in ass;
	int sockfd = 0;
	sockfd = createSocket_client();

	ass.sin_family = AF_INET;
	ass.sin_addr.s_addr = INADDR_ANY;
	ass.sin_port = htons(PORT);

	bind(sockfd, (struct sockaddr *) &ass, sizeof(ass));

	cse4589_print_and_log("Bind function on client called");
	cse4589_print_and_log("\n");
}

void connect_client()
{
	struct sockaddr_in ass;
	int sockfd = 0;
	sockfd = createSocket_client();

	ass.sin_family = AF_INET;
	ass.sin_addr.s_addr = INADDR_ANY;
	ass.sin_port = htons(PORT);

	connect(sockfd, (struct sockaddr *) &ass, sizeof(ass));

	cse4589_print_and_log("Connect function on client called");
	cse4589_print_and_log("\n");
}