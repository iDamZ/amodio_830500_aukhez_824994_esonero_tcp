/*
 * main.c
 *
 * TCP Weather Server
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
#include <time.h>

#include "protocol.h"

#define NO_ERROR 0

void clearwinsock() {
#if defined WIN32
    WSACleanup();
#endif
}


float get_temperature(void) {
    return -10.0f + (float)rand() / RAND_MAX * (40.0f - (-10.0f));
}

float get_humidity(void) {
    return 20.0f + (float)rand() / RAND_MAX * (100.0f - 20.0f);
}

float get_wind(void) {
    return (float)rand() / RAND_MAX * 100.0f;
}

float get_pressure(void) {
    return 950.0f + (float)rand() / RAND_MAX * (1050.0f - 950.0f);
}


int main(int argc, char *argv[]) {

#if defined WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
    if (result != NO_ERROR) {
        printf("Error at WSAStartup()\n");
        return 0;
    }
#endif

    srand(time(NULL)); // Random generator

    int my_socket;

    //  Creazione socket
    my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (my_socket < 0) {
        perror("socket creation failed.\n");
        clearwinsock();
        return -1;
    }

    //  Configurazione indirizzo server

    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");
    sad.sin_port = htons(SERVER_PORT);

    //  Bind socket
    if (bind(my_socket, (struct sockaddr*)&sad, sizeof(sad)) < 0) {
        perror("bind() failed.\n");
        closesocket(my_socket);
        clearwinsock();
        return -1;
    }

    // 4) Listen
    if (listen(my_socket, QLEN) < 0) {
        perror("listen() failed.\n");
        closesocket(my_socket);
        clearwinsock();
        return -1;
    }

    printf("Weather server running on port %d...\n", SERVER_PORT);

    // 5) Loop di accettazione

    struct sockaddr_in cad;
    int client_socket;
    int client_len;

    while (1) {
        printf("Waiting for a client...\n");

        client_len = sizeof(cad);
        client_socket = accept(my_socket, (struct sockaddr*)&cad, &client_len);

        if (client_socket < 0) {
            perror("accept() failed\n");
            continue;
        }

        printf("Client connected: %s\n", inet_ntoa(cad.sin_addr));

        // 6) Gestione della richiesta

        weather_request_t req;
        int bytes = recv(client_socket, &req, sizeof(req), 0);

        if (bytes <= 0) {
            printf("Invalid request or connection closed.\n");
            closesocket(client_socket);
            continue;
        }

        weather_response_t res;
        res.status = 0; // default = success
        res.type = req.type;

        // Validazione tipo richiesto
        if (req.type != 't' && req.type != 'h' &&
            req.type != 'w' && req.type != 'p') {

            res.status = 2; // richiesta non valida
            res.value = 0;
        }
        else if (strlen(req.city) == 0) {
            res.status = 1; // città non disponibile
        }
        else {
            // Tipo valido → genera valore
            switch (req.type) {
                case 't': res.value = get_temperature(); break;
                case 'h': res.value = get_humidity();    break;
                case 'w': res.value = get_wind();        break;
                case 'p': res.value = get_pressure();    break;
            }
        }

        //  Invio risposta
        send(client_socket, &res, sizeof(res), 0);

        printf("Request served. Closing connection.\n");
        closesocket(client_socket);
    }

    closesocket(my_socket);
    clearwinsock();
    return 0;
}
