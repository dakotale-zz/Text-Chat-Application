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
// For uppercase (toupper function)
// May not be needed
//#include <ctype.h>
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

#include "../include/server.h"
#include "../include/client.h"

#define BUF_LEN 128

void shellloop();
void author();
void ip();
void port();
void listclients();
void exitprogram();

/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */
int main(int argc, char **argv)
{
	/*Init. Logger*/
	cse4589_init_log(argv[2]);

	/*Clear LOGFILE*/
	fclose(fopen(LOGFILE, "w"));

	// 0 meaning the server is requested
	// 1 meaning the client is requested
	int isclient = 0;

	int sockfd = 0;

	/*Start Here*/
	if (strcmp(argv[1], "s") == 0)
	{
		sockfd = createSocket_server();
		bind_server();
		listen_server();
		accept_server();
	}
	if (strcmp(argv[1], "c") == 0)
	{
		isclient = 1;
		sockfd = createSocket_client();
		bind_client();
		connect_client();
	}
	shellloop();

	return 0;
}

void shellloop()
{
	// Basic shell, referenced from https://stackoverflow.com/questions/4788374/writing-a-basic-shell
	char line[BUF_LEN];
	char *command;

	while (1) 
	{
		printf("$ ");

		if (!fgets(line, BUF_LEN, stdin)) 
			break;

		command = strtok(line, "\n");

		// Read inputs and call associated methods
		if (strcmp(command, "AUTHOR") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			author();
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strcmp(command, "IP") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			ip();
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strcmp(command, "EXIT") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			exitprogram();
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strcmp(command, "PORT") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			port();
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else if (strcmp(command, "LIST") == 0)
		{
			cse4589_print_and_log("[%s:SUCCESS]\n", command);
			listclients();
			cse4589_print_and_log("[%s:END]\n", command);
		}
		else
		{
			cse4589_print_and_log("[%s:ERROR]\n", command);
			cse4589_print_and_log("[%s:END]\n", command);
		}

	}

}

void author()
{
	char your_ubit_name[9] = "dakotale";
	cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", your_ubit_name);
}

void ip()
{
	// Adapted from https://www.geeksforgeeks.org/c-program-display-hostname-ip-address/
	char hostbuffer[32];
    char *IP;
    struct hostent *host;
    int hostname;
 
    // To retrieve hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
 
    // To retrieve host information
    host = gethostbyname(hostbuffer);
 
    // To convert an Internet network
    // address into ASCII string
    // Also referencing https://beej.us/guide/bgnet/html/multi/inet_ntoaman.html
    IP = inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));
 
    cse4589_print_and_log(IP);
    cse4589_print_and_log("\n");
}

void port()
{
	printf("This part requires socket API.\n");
}

void listclients()
{
	printf("This part requires socket API.\n");
}

void exitprogram()
{
	exit(0);
}