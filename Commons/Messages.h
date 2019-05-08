#ifndef PRIMER_PROYECTO_MESSAGE_H
#define PRIMER_PROYECTO_MESSAGE_H

#include <map>
#include <sstream>

enum INSTANCE_TYPE {
    WEATHER_MICROSERVICE,
    EXCHANGE_MICROSERVICE,
    CLIENT,
    PORTAL,
    MS_QUERY_CONTROLLER
};

enum Method {
    READ,
    WRITE
};

const std::map<char, Method> CHAR_TO_METHOD = {{'r', READ}, {'w', WRITE}};
const std::map<char, INSTANCE_TYPE> CHAR_TO_SERVICE = {{'w', WEATHER_MICROSERVICE}, {'c', EXCHANGE_MICROSERVICE}};
static const char *serviceNames[] = {"WEATHER_MICROSERVICE", "EXCHANGE_MICROSERVICE", "CLIENT", "PORTAL",
                                     "MS_QUERY_CONTROLLER"};
static const char *methodNames[] = {"READ", "WRITE"};

struct WeatherRecord {
    float temperature;
    float pressure;
    float humidity;
    char name[20];
    char code[3];

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
    char code[3];

    std::string asString() {
        std::stringstream response_message;
        response_message  << "name: " <<  name <<"; exchange: " << exchange;
        return response_message.str();
    }
};

struct PortalResponse {
    INSTANCE_TYPE instanceType;
    union {
        WeatherRecord weatherRecord;
        ExchangeRecord exchangeRecord;
    };
    bool found;
    bool requestError = false;

    std::string asString() {
        std::stringstream response_message;
        response_message  << "instanceType: " <<  serviceNames[instanceType] <<"; found: "
                          << found << "; record: ";

        switch(instanceType) {
            case WEATHER_MICROSERVICE: {
                response_message << weatherRecord.asString() << std::endl;
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                response_message << exchangeRecord.asString() << std::endl;
                break;
            }
            default:
                response_message << "Unexpected logWriter type" << std::endl;
                break;
        }
        return response_message.str();
    }
};

struct MSRequest {
    Method method;
    INSTANCE_TYPE instanceType;
    char responseFifoPath[50];
    char code[3];
    bool closeConnection;
    float temperature;
    float pressure;
    float humidity;
    float exchange;
    union {
        WeatherRecord weatherRecord;
        ExchangeRecord exchangeRecord;
    };

    std::string asString() {
        std::string representation = "Method: ";
        representation = representation + methodNames[method] + ", Code: " + code;
        representation = representation + ", responseFifoPath: " + responseFifoPath;
        if (instanceType == WEATHER_MICROSERVICE) {
            representation = representation + ", Weather Data: " + weatherRecord.asString();
        } else {
            representation = representation + ", Exchange Data: " + exchangeRecord.asString();
        }
        if (closeConnection) {
            representation = representation + ", closeConnection: true";
        } else {
            representation = representation + ", closeConnection: false";
        }
        return representation;
    }
};

struct ConnectionRequest {
    char senderResponseFifoPath[50];
    INSTANCE_TYPE instanceType;
};

#endif //PRIMER_PROYECTO_MESSAGE_H
