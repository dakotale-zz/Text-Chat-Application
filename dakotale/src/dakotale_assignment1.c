/**
 * @dakotale_assignment1
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
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
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
#include <fcntl.h>

#include "../include/global.h"
#include "../include/logger.h"

#define BUF_LEN 256

#define BACKLOG 5
#define PORT 27000
#define SERVPORT 27001

void shellloop();

/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */

// Works for one socket and one server 
// Has to work for multiple clients in order to proceed
int main(int argc, char **argv)
{
	/*Init. Logger*/
	cse4589_init_log(argv[2]);

	/*Clear LOGFILE*/
	fclose(fopen(LOGFILE, "w"));

	/* Start here */
	char *po;
	long port = strtol(argv[2], &po, 10);

	// Commands used for the user input
	char line[BUF_LEN];
	char *command;

	char buffer[BUF_LEN];
	// Allocate 256 bytes for the message
	char *msg;
	msg = (char *)malloc(sizeof(char)*BUF_LEN);

	// Initialize the client and server sockets
	int clisock, servsock = 0;
	// Used for the client address length and server accept
	int caddr_len, fdaccept, num = 0;
	
	// Socket information for the client and server
	struct sockaddr_in client_addr, server_addr;

	// Assign the client socket
	clisock = socket(AF_INET, SOCK_STREAM, 0);

	// Assign the server socket
	servsock = socket(AF_INET, SOCK_STREAM, 0);

	// Mark the space as all zeroes to prevent any segmentation faults
	bzero(&client_addr, sizeof(client_addr));
	bzero(&server_addr, sizeof(server_addr));

	// Provide IPv4 details and port info for the client struct
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = htons(INADDR_ANY);
	if(strcmp(argv[1], "c") == 0)
		client_addr.sin_port = htons(port);
	else
		client_addr.sin_port = htons(PORT);

	// Provide IPv4 details and port info for the server struct
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);

	if(strcmp(argv[1], "s") == 0)
		server_addr.sin_port = htons(port);
	else
		server_addr.sin_port = htons(SERVPORT);

	// Attempt to bind the client socket with the given port
	if(bind(clisock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
		cse4589_print_and_log("Client Bind failed");

	// Attempt to bind the server socket with the given port
	if(bind(servsock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		cse4589_print_and_log("Server Bind failed");

	// Server listens for any incoming connections made to it
	if(listen(servsock, BACKLOG) < 0)
		cse4589_print_and_log("Can't listen to port %d", PORT);

	// Adapted from https://www.geeksforgeeks.org/c-program-display-hostname-ip-address/
	char hostbuffer[256];
    char *IP;
    struct hostent *host;
    int hostname;

    // To retrieve hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
 
    // To retrieve host information
    host = gethostbyname(hostbuffer);
    // End of reference
 
    // To convert an Internet network
    // address into ASCII string
    // Also referencing https://beej.us/guide/bgnet/html/multi/inet_ntoaman.html
    IP = inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));

	int ipinfo, portinfo = 0;

	// SHELL
	while (1) 
	{
		char hostname[256];
		gethostname(hostname, 256);
		
		printf("$ ");

		if (!fgets(line, BUF_LEN, stdin)) 
			break;

		command = strtok(line, "\n");
	
		// Quick comparison of the substring for IP and port for LOGIN
		if(strstr(command, IP))
			ipinfo = 1;

		if(strstr(command, po) && po != NULL)
			portinfo = 1;

		// Read inputs and call associated methods
		if (strcmp(command, "AUTHOR") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			char your_ubit_name[9] = "dakotale";
			cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", your_ubit_name);
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strcmp(command, "IP") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			cse4589_print_and_log("IP:%s\n", IP);
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strcmp(command, "PORT") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			cse4589_print_and_log("PORT:%d\n", port);
			cse4589_print_and_log("[%s:END]\n", command);
		}
		// Has to be LOGIN <serverip> <server port>
		else if (strstr(command, "LOGIN") && portinfo == 1 && ipinfo == 1) 
		{
			if(connect(clisock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
				cse4589_print_and_log("Can't connect");
			else
			{
				cse4589_print_and_log("Connected");
				cse4589_print_and_log("\n");
				cse4589_print_and_log("Accepting");
				cse4589_print_and_log("\n");
				caddr_len = sizeof(client_addr);
				fdaccept = accept(servsock, (struct sockaddr *)&client_addr, &caddr_len);
				// If the server cannot accept the connection
				if(fdaccept < 0)
					cse4589_print_and_log("Can't accept");
				// The server connection is accepted
				else
				{
					cse4589_print_and_log("Server connection accepted");
					cse4589_print_and_log("\n");
				}
			}
		}
		else if (strcmp(command, "LIST") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", clisock, hostname, IP, port);
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strcmp(command, "STATISTICS") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			cse4589_print_and_log("%-5d%-35s%-8d%-8d%-8s\n", clisock, hostname, 0, 0, "logged-in");
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strcmp(command, "REFRESH") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", clisock, hostname, IP, port);
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strstr(command, "BLOCK") && ipinfo == 1)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			// From : http://man7.org/linux/man-pages/man2/fcntl.2.html
			fcntl(clisock, F_SETFL, EWOULDBLOCK);
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strstr(command, "UNBLOCK") && ipinfo == 1)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			// From Beej on 7.1: Blocking
			// Link: https://beej.us/guide/bgnet/html/multi/advanced.html#blocking
			fcntl(clisock, F_SETFL, O_NONBLOCK);
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strcmp(command, "LOGOUT") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			close(clisock);
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strcmp(command, "EXIT") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			close(clisock);
			cse4589_print_and_log("[%s:END]\n", command);
			exit(0);
		}
		else
		{
			cse4589_print_and_log("[%s:ERROR]\n", command);
			cse4589_print_and_log("[%s:END]\n", command);
		}
	}

	return 0;

}

