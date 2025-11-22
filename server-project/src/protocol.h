#ifndef PROTOCOL_H
#define PROTOCOL_H


typedef struct {
    char type;
    char city[64];
} weather_request_t;

typedef struct {
    unsigned int status;
    char type;
    float value;
} weather_response_t;


#define SERVER_PORT 56700
#define QLEN 6


float get_temperature(void);
float get_humidity(void);
float get_wind(void);
float get_pressure(void);

#endif
