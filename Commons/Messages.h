#ifndef PRIMER_PROYECTO_MESSAGE_H
#define PRIMER_PROYECTO_MESSAGE_H

#include <map>
#include <sstream>

enum Service {
    WEATHER,
    EXCHANGE
};

enum Method {
    READ,
    WRITE
};

const std::map<char, Method> CHAR_TO_METHOD = {{'r', READ}, {'w', WRITE}};
const std::map<char, Service> CHAR_TO_SERVICE = {{'w', WEATHER}, {'c', EXCHANGE}};
static const char *serviceNames[] = {"WEATHER", "CHANGE"};
static const char *methodNames[] = {"READ", "WRITE"};

struct WeatherRecord {
    float temperature;
    float pressure;
    float humidity;
    char name[20];

    std::string asString() {
        std::stringstream response_message;
        response_message  << "name: " <<  name <<"; temperature: " << temperature << "; pressure: " << pressure
                          << "; humidity: " << humidity;
        return response_message.str();
    }
};

struct ExchangeRecord {
    float exchange;
    char name[20];

    std::string asString() {
        std::stringstream response_message;
        response_message  << "name: " <<  name <<"; exchange: " << exchange;
        return response_message.str();
    }
};

struct PortalResponse {
    Service service;
    union {
        WeatherRecord weatherRecord;
        ExchangeRecord exchangeRecord;
    };
    bool found;

    std::string asString() {
        std::stringstream response_message;
        response_message  << "service: " <<  serviceNames[service] <<"; found: "
                          << found << "; record: ";
        if (service == WEATHER) {
            response_message << weatherRecord.asString() << std::endl;
        }
        return response_message.str();
    }
};

struct MSRequest {
    Method method;
    Service service;
    union {
        WeatherRecord weatherRecord;
        ExchangeRecord exchangeRecord;
    };
    char responseFifoPath[50];
    char code[3];
    bool closeConnection;

    std::string asString() {
        std::string representation = "Method: ";
        representation = representation + methodNames[method] + ", Code: " + code;
        if (service == WEATHER) {
            representation = representation + ", Data: " + weatherRecord.asString();
        }
        if (closeConnection) {
            representation = representation + ", closeConnection: true";
        } else {
            representation = representation + ", closeConnection: false";
        }
        return representation;
    }
};

#endif //PRIMER_PROYECTO_MESSAGE_H
