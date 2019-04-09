#ifndef PRIMER_PROYECTO_MESSAGE_H
#define PRIMER_PROYECTO_MESSAGE_H

#include <map>
#include <sstream>

enum Service {
    WEATHER,
    CHANGE
};

enum Method {
    READ,
    WRITE
};

const std::map<char, Method> CHAR_TO_METHOD = {{'r', READ}, {'w', WRITE}};
const std::map<char, Service> CHAR_TO_SERVICE = {{'w', WEATHER}, {'c', CHANGE}};
static const char *serviceNames[] = {"WEATHER", "CHANGE"};
static const char *methodNames[] = {"READ", "WRITE"};


struct portal_request_message_t {
    Service service;
    Method method;
    char code[3];

    std::string asString() {
        std::string representation = "Service: ";
        representation = representation + serviceNames[service] + ", Method: " + methodNames[method];
        representation = representation + ", Code: " + code;
        return representation;
    }
};

struct portal_response_message_t {
    Service service;
    float exchange_rate;
    float temperature;
    float pressure;
    float humidity;
    bool found;

    std::string asString() {
        std::stringstream response_message;
        response_message  << "Temperature: " << temperature <<"; Pressure: "
                          << pressure << "; Humidity: " << humidity << std::endl;
        return response_message.str();
    }
};

struct WeatherRequest {
    Method method;
    char code[3];
    float temperature;
    float pressure;
    float humidity;
    char name[20];
    char responseFifoPath[50];
    bool closeConnection;

    std::string asString() {
        std::string representation = "Method: ";
        representation = representation + methodNames[method] + ", Name: " + name + ", Code: " + code;
        if (closeConnection) {
            representation = representation + ", closeConnection: true";
        } else {
            representation = representation + ", closeConnection: false";
        }
        return representation;
    }
};


#endif //PRIMER_PROYECTO_MESSAGE_H
