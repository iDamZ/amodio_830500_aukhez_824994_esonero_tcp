/*
 * main.c
 *
 * TCP Client - Template for Computer Networks assignment
 *
 * This file contains the boilerplate code for a TCP client
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
#include <errno.h>
#include <stdlib.h>
#include "protocol.h"

#define NO_ERROR 0

/* --- DEFINIZIONE STRUTTURE (PROTOCOLLO) --- */

// Messaggio di Richiesta (Client -> Server)
typedef struct {
    char type;        // 't', 'h', 'w', 'p'
    char city[64];    // Nome città (stringa null-terminated)
} richiesta_client;

// Messaggio di Risposta (Server -> Client)
typedef struct {
    unsigned int status;  // 0=Ok, 1=No City, 2=Invalid
    char type;            // Eco del tipo
    float value;          // Valore numerico
} risposta_server;

void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

int main(int argc, char *argv[]) {

	// TODO: Implement client logic

	char *server_ip = SERVER_IP;
	   int port = SERVER_PORT;
	char* input_string = NULL; // Stringa da inviare

	for (int i = 1; i < argc; i++) {
	        if (strcmp(argv[i], "-s") == 0 && (i + 1) < argc) {
	            server_ip = argv[++i];
	        } else if (strcmp(argv[i], "-p") == 0 && (i + 1) < argc) {
	            port = atoi(argv[++i]);
	        } else if (strcmp(argv[i], "-r") == 0 && (i + 1) < argc) {
	            input_string = argv[++i];
	        }
	    }

	    // Controllo obbligatorio: se manca -r, terminiamo
	    if (input_string == NULL) {
	        fprintf(stderr, "Errore: parametro -r obbligatorio\n");
	        return -1;
	    }

	        // Esempio: "t bari" -> type='t', city="bari"
	        richiesta_client req;
	        memset(&req, 0, sizeof(req));
	        req.type = input_string[0];
	        if (strlen(input_string) > 1) {
	            char *city_start = input_string + 1;
	            // Se c'è uno spazio subito dopo il tipo, lo saltiamo
	            if (*city_start == ' ') {
	                city_start++;
	            }
	            strncpy(req.city, city_start, 63);
	        } else {
	            req.city[0] = '\0';
	        }


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
	my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (my_socket < 0) {
	perror("socket creation failed.\n");
	closesocket(my_socket);
	clearwinsock();
	return -1;
	}
	// TODO: Configure server address
	// struct sockaddr_in server_addr;

	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr(server_ip);
	sad.sin_port = htons(port);

	// ...

	// TODO: Connect to server
	// connect(...);

	if (connect(my_socket, (struct sockaddr *)&sad, sizeof(sad))< 0)
	{
	perror( "Failed to connect.\n" );
	closesocket(my_socket);
	clearwinsock();
	return -1;
	}


	// TODO: Implement communication logic
	// send(...);
	// recv(...);

	if (send(my_socket, &req, sizeof(req), 0) < 0) {
	        perror("Send failed");
	        close(my_socket);
	        return -1;
	    }



	int bytes_rcvd;
	int total_bytes_rcvd = 0;
	printf("Received: "); // Setup to print the echoed string
	risposta_server resp;
	while (total_bytes_rcvd < sizeof(req)) {
	if ((bytes_rcvd = recv(my_socket, &resp, sizeof(resp), 0)) <= 0)
	{
	perror("Receive failed");
	closesocket(my_socket);
	clearwinsock();
	return -1;
	}
	total_bytes_rcvd += bytes_rcvd; // Keep tally of total bytes
	}

	//Stampa Formattata
	if (resp.status == 0) {
		printf("Ricevuto risultato dal server ip %s. ", server_ip);
	        switch (req.type) {
	            case 't':
	                printf("%s: Temperatura = %.1f°C\n", req.city, resp.value);
	                break;
	            case 'h':
	                printf("%s: Umidità = %.1f%%\n", req.city, resp.value);
	                break;
	            case 'w':
	                printf("%s: Vento = %.1f km/h\n", req.city, resp.value);
	                break;
	            case 'p':
	                printf("%s: Pressione = %.1f hPa\n", req.city, resp.value);
	                break;
	            default:
	                // Caso in cui il server risponde successo ma il tipo non è riconosciuto dal client
	                printf("Tipo sconosciuto\n");
	                break;
	        }
	    } else if (resp.status == 1) {
	        // ERRORE: Città non disponibile
	        printf("Città non disponibile\n");
	    } else {
	        // ERRORE: Richiesta non valida (status 2 o altro)
	        printf("Richiesta non valida\n");
	    }

	// TODO: Close socket
	// closesocket(my_socket);
	closesocket(my_socket);
	clearwinsock();
	printf("\n"); // Print a final linefeed
	system("pause");
	return(0);
	}
