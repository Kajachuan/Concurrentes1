#ifndef PRIMER_PROYECTO_MESSAGE_H
#define PRIMER_PROYECTO_MESSAGE_H

#include <map>
#include <sstream>
#define MAXBUFF 50

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
        response_message << "name: " << name << "; temperature: " << temperature << "; pressure: "
                         << pressure << "; humidity: " << humidity;
        return response_message.str();
    }

    void serialize(char* serialized) {
        float* floated_serialized = (float*) serialized;
        *floated_serialized = temperature;
        floated_serialized++;
        *floated_serialized = pressure;
        floated_serialized++;
        *floated_serialized = humidity;
        floated_serialized++;
        char* char_serialized = (char*) floated_serialized;
        for (int i = 0; i < strlen(code); i++) {
            *char_serialized = code[i];
            char_serialized++;
        }
        for (int i = 0; i < strlen(name); i++) {
            *char_serialized = name[i];
            char_serialized++;
        }
    }

    void deserialize(const char* serialized) {
        float* floated_serialized = (float*) serialized;
        temperature = *floated_serialized;
        floated_serialized++;
        pressure = *floated_serialized;
        floated_serialized++;
        humidity = *floated_serialized;
        floated_serialized++;
        char* char_serialized = (char*) floated_serialized;
        for (int i = 0; i < 3; i++) {
            code[i] = *char_serialized;
            char_serialized++;
        }
        for (int i = 0; i < strlen(char_serialized); i++) {
            name[i] = *char_serialized;
            char_serialized++;
        }
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

    void serialize(char* serialized) {
        float* float_serialized = (float*) serialized;
        *float_serialized = exchange;
        char* char_serialized = (char*) float_serialized;
        for (int i = 0; i < strlen(code); i++) {
            *char_serialized = code[i];
            char_serialized++;
        }
        for (int i = 0; i < strlen(name); i++) {
            *char_serialized = name[i];
            char_serialized++;
        }
    }

    void deserialize(const char* serialized) {
        float* floated_serialized = (float*) serialized;
        exchange = *floated_serialized;
        floated_serialized++;
        char* char_serialized = (char*) floated_serialized;
        for (int i = 0; i < 3; i++) {
            code[i] = *char_serialized;
            char_serialized++;
        }
        for (int i = 0; i < strlen(char_serialized); i++) {
            name[i] = *char_serialized;
            char_serialized++;
        }
    }
};

struct PortalResponse {
    INSTANCE_TYPE instanceType;
    union {
        WeatherRecord weatherRecord;
        ExchangeRecord exchangeRecord;
    };
    bool found = false;
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
                response_message << "Unexpected instance type" << std::endl;
                break;
        }
        return response_message.str();
    }

    char* serialize() {
        char serialized[MAXBUFF];
        INSTANCE_TYPE* enum_serialized = (INSTANCE_TYPE*) serialized;
        *enum_serialized = instanceType;
        enum_serialized++;
        bool* bool_serialized = (bool*) enum_serialized;
        *bool_serialized = found;
        bool_serialized++;
        *bool_serialized = requestError;
        bool_serialized++;
        char* char_serialized = (char*) bool_serialized;
        switch(instanceType) {
            case WEATHER_MICROSERVICE: {
                weatherRecord.serialize(char_serialized);
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                exchangeRecord.serialize(char_serialized);
                break;
            }
            default:
                break;
        }
        return serialized;
    }

    void deserialize(const char* serialized) {
        INSTANCE_TYPE* enum_serialized = (INSTANCE_TYPE*) serialized;
        instanceType = *enum_serialized;
        enum_serialized++;
        bool* bool_serialized = (bool*) enum_serialized;
        found = *bool_serialized;
        bool_serialized++;
        *bool_serialized = requestError;
        bool_serialized++;
        char* char_serialized = (char*) bool_serialized;
        switch(instanceType) {
            case WEATHER_MICROSERVICE: {
                weatherRecord.deserialize(char_serialized);
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                exchangeRecord.deserialize(char_serialized);
                break;
            }
            default:
                break;
        }
    }
};

struct MSRequest {
    Method method;
    INSTANCE_TYPE instanceType;
    char responseFifoPath[50];
    char code[3];
    bool closeConnection;
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
    char* serialize() {
        char serialized[MAXBUFF];
        Method* method_serialized = (Method*) serialized;
        *method_serialized = method;
        method_serialized++;
        INSTANCE_TYPE* type_serialized = (INSTANCE_TYPE*) serialized;
        *type_serialized = instanceType;
        type_serialized++;
        bool* bool_serialized = (bool*) type_serialized;
        *bool_serialized = closeConnection;
        bool_serialized++;
        int* int_serialized = (int*) bool_serialized;
        *int_serialized = static_cast<int>(strlen(responseFifoPath));
        int_serialized++;
        char* char_serialized = (char*) int_serialized;
        for (int i = 0; i < strlen(responseFifoPath); i++) {
            *char_serialized = responseFifoPath[i];
            char_serialized++;
        }
        for (int i = 0; i < strlen(code); i++) {
            *char_serialized = code[i];
            char_serialized++;
        }
        switch(instanceType) {
            case WEATHER_MICROSERVICE: {
                weatherRecord.serialize(char_serialized);
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                exchangeRecord.serialize(char_serialized);
                break;
            }
            default:
                break;
        }
        return serialized;
    }

    void deserialize(const char* serialized) {
        Method* method_serialized = (Method*) serialized;
        method = *method_serialized;
        method_serialized++;
        INSTANCE_TYPE* type_serialized = (INSTANCE_TYPE*) serialized;
        instanceType = *type_serialized;
        type_serialized++;
        bool* bool_serialized = (bool*) type_serialized;
        closeConnection = *bool_serialized;
        bool_serialized++;
        int* int_serialized = (int*) bool_serialized;
        int responseFifoPathLength = *int_serialized;
        int_serialized++;
        char* char_serialized = (char*) int_serialized;
        for (int i = 0; i < responseFifoPathLength; i++) {
            responseFifoPath[i] = *char_serialized;
            char_serialized++;
        }
        for (int i = 0; i < 3; i++) {
            code[i] = *char_serialized;
            char_serialized++;
        }
        switch(instanceType) {
            case WEATHER_MICROSERVICE: {
                weatherRecord.deserialize(char_serialized);
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                exchangeRecord.deserialize(char_serialized);
                break;
            }
            default:
                break;
        }
    }
};

struct ConnectionRequest {
    char senderResponseFifoPath[50];
    INSTANCE_TYPE instanceType;
};

#endif //PRIMER_PROYECTO_MESSAGE_H
