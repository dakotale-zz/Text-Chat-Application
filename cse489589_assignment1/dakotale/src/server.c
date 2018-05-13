/*
 * @server
 * @author  Dakota Lester <dakotale@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * 
 * @section DESCRIPTION
 *
 * This contains the server function using as a reference Beej's server.c file
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "../include/global.h"
#include "../include/logger.h"

#define PORT 3490

int createSocket_server()
{
	int sockfd = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		cse4589_print_and_log("Can't open socket");

	cse4589_print_and_log("Create function on server called");
	cse4589_print_and_log("\n");

	return sockfd;
}

void bind_server()
{
	struct sockaddr_in ass;
	int sockfd = 0;
	sockfd = createSocket_server();

	ass.sin_family = AF_INET;
	ass.sin_addr.s_addr = INADDR_ANY;
	ass.sin_port = htons(PORT);

	bind(sockfd, (struct sockaddr *) &ass, sizeof(ass));
	cse4589_print_and_log("Bind function on server called");
	cse4589_print_and_log("\n");
}

void listen_server()
{
	int sockfd = 0;
	sockfd = createSocket_server();
	listen(sockfd, SOMAXCONN);
	cse4589_print_and_log("Listen function on server called");
	cse4589_print_and_log("\n");
}

void accept_server()
{
	struct sockaddr_in ass;
	int sockfd = 0;
	int asslen;
	sockfd = createSocket_server();

	ass.sin_family = AF_INET;
	ass.sin_addr.s_addr = INADDR_ANY;
	ass.sin_port = htons(PORT);

	asslen = sizeof(ass);
	accept(sockfd, (struct sockaddr *) &ass, &asslen);

	cse4589_print_and_log("Accept function on server called");
	cse4589_print_and_log("\n");
}
