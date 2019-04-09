#ifndef PRIMER_PROYECTO_MESSAGE_H
#define PRIMER_PROYECTO_MESSAGE_H


struct portal_request_message_t {
    char service;
    char method;
    char code[3];
};

struct portal_response_message_t {
    char service;
    float exchange_rate;
    float temperature;
    float pressure;
    float humidity;
};


#endif //PRIMER_PROYECTO_MESSAGE_H
