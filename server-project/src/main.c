/*
 * main.c
 *
 * TCP Server - Template for Computer Networks assignment
 *
 * This file contains the boilerplate code for a TCP server
 * portable across Windows, Linux and macOS.
 */

#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "protocol.h"

#define NO_ERROR 0

void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

int main(int argc, char *argv[]) {

	// TODO: Implement server logic

#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

	int my_socket;



	// TODO: Create socket
	// my_socket = socket(...);
	my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (my_socket < 0) {
	perror("socket creation failed.\n");
	clearwinsock();
	return -1;
	}


	// TODO: Configure server address
	// struct sockaddr_in server_addr;
	// server_addr.sin_family = AF_INET;
	// server_addr.sin_port = htons(SERVER_PORT);
	// server_addr.sin_addr.s_addr = INADDR_ANY;
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(56700); /* converts values between the host and*/

	// TODO: Bind socket
	// bind(...);
	if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
	perror("bind() failed.\n");
	closesocket(my_socket);
	clearwinsock();
	return -1;
	}
	// TODO: Set socket to listen
	// listen(...);
	if (listen (my_socket, QLEN) < 0) {
	perror("listen() failed.\n");
	closesocket(my_socket);
	clearwinsock();
	return -1;
	}

	// TODO: Implement connection acceptance loop
	// while (1) {
	//     int client_socket = accept(...);
	//     // Handle client communication
	//     closesocket(client_socket);
	// }
	struct sockaddr_in cad; // structure for the client address
	int client_socket; // socket descriptor for the client
	int client_len; // the size of the client address
	printf("Waiting for a client to connect...");
	while (1) {
	client_len = sizeof(cad); // set the size of the client address
	if ((client_socket = accept(my_socket, (struct sockaddr*)&cad, &client_len)) < 0)
	{
	perror("accept() failed.\n");

	// CHIUSURA DELLA CONNESSIONE
	printf("Server terminated.\n");
	closesocket(client_socket);
	clearwinsock();
	return 0;
	}
	printf("Handling client %s\n", inet_ntoa(cad.sin_addr));

	} // end-while
	} // end-main


