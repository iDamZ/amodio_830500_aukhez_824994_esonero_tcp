/*
 * protocol.h
 *
 * Client header file
 * Definitions, constants and function prototypes for the client
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/* --- DEFINIZIONE STRUTTURE (PROTOCOLLO) --- */

// Messaggio di Richiesta (Client -> Server)
typedef struct {
    char type;        // 't', 'h', 'w', 'p'
    char city[64];    // Nome città (stringa null-terminated)
} weather_request_t;

// Messaggio di Risposta (Server -> Client)
typedef struct {
    unsigned int status;  // 0=Ok, 1=No City, 2=Invalid
    char type;            // Eco del tipo
    float value;          // Valore numerico
} weather_response_t;

// Shared application parameters
#define SERVER_PORT 56700  // Server port (change if needed)
#define SERVER_IP "127.0.0.1"  // Server port (change if needed)
#define BUFFER_SIZE 512    // Buffer size for messages

// Function prototypes
// Add here the signatures of the functions implemented by students

/*
 * Example function to implement:
 * int connect_to_server(const char* server_address);
 */

#endif /* PROTOCOL_H_ */
