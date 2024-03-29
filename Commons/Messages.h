#ifndef PRIMER_PROYECTO_MESSAGE_H
#define PRIMER_PROYECTO_MESSAGE_H

#include <map>
#include <sstream>
#include <string.h>

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
	    strcpy(char_serialized, code);
	    strcpy(&char_serialized[3], name);
    }

    void deserialize(const char* serialized, int total_size) {
        float* floated_serialized = (float*) serialized;
        temperature = *floated_serialized;
        floated_serialized++;
        pressure = *floated_serialized;
        floated_serialized++;
        humidity = *floated_serialized;
        floated_serialized++;
        char* char_serialized = (char*) floated_serialized;
	    for (int i = 0; i < 3; i++) {
		    code[i] = char_serialized[i];
	    }
	    for (int i = 0; i < 20; i++) {
		    name[i] = char_serialized[3 + i];
	    }
    }

    size_t get_bytes_size() {
        return sizeof(float) * 3 + 3 * sizeof(char) + 20 * sizeof(char);
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
	    float_serialized++;
        char* char_serialized = (char*) float_serialized;
	    strcpy(char_serialized, code);
	    strcpy(&char_serialized[3], name);
    }

    void deserialize(const char* serialized, int total_size) {
        float* floated_serialized = (float*) serialized;
        exchange = *floated_serialized;
        floated_serialized++;
        char* char_serialized = (char*) floated_serialized;
	    for (int i = 0; i < 3; i++) {
		    code[i] = char_serialized[i];
	    }
	    for (int i = 0; i < 20; i++) {
		    name[i] = char_serialized[3 + i];
	    }
    }

    size_t get_bytes_size() {
        return sizeof(float) + 3 * sizeof(char) + 20 * sizeof(char);
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
                response_message << "Unexpected logWriter type" << std::endl;
                break;
        }
        return response_message.str();
    }

    char* serialize(char* serialized) {
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
    }

    void deserialize(const char* serialized, int total_size) {
        INSTANCE_TYPE* enum_serialized = (INSTANCE_TYPE*) serialized;
        instanceType = *enum_serialized;
        enum_serialized++;
        bool* bool_serialized = (bool*) enum_serialized;
        found = *bool_serialized;
        bool_serialized++;
        requestError = *bool_serialized;
        bool_serialized++;
        char* char_serialized = (char*) bool_serialized;
        switch(instanceType) {
            case WEATHER_MICROSERVICE: {
                weatherRecord.deserialize(char_serialized, total_size);
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                exchangeRecord.deserialize(char_serialized, total_size);
                break;
            }
            default:
                break;
        }
    }

    size_t get_bytes_size() {
        size_t common_size = sizeof(INSTANCE_TYPE) + sizeof(bool) * 2;
        size_t instance_size;
        switch(instanceType) {
            case WEATHER_MICROSERVICE: {
                instance_size = weatherRecord.get_bytes_size();
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                instance_size = exchangeRecord.get_bytes_size();
                break;
            }
            default:
                instance_size = 0;
                break;
        }
        return common_size + instance_size;
    }

    void serialize_with_size(char* serialized, size_t size) {
        int* int_serialized = (int*) serialized;
        *int_serialized = static_cast<int>(size - sizeof(int));
        int_serialized++;
        char* request_serialized = (char*) int_serialized;
        serialize(request_serialized);
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

    char* serialize(char* serialized) {
        Method* method_serialized = (Method*) serialized;
        *method_serialized = method;
        method_serialized++;
        INSTANCE_TYPE* type_serialized = (INSTANCE_TYPE*) method_serialized;
        *type_serialized = instanceType;
        type_serialized++;
        bool* bool_serialized = (bool*) type_serialized;
        *bool_serialized = closeConnection;
        bool_serialized++;
        char* char_serialized = (char*) bool_serialized;
	    strcpy(char_serialized, responseFifoPath);
	    strcpy(&char_serialized[50], code);
        switch(instanceType) {
            case WEATHER_MICROSERVICE: {
                weatherRecord.serialize(&char_serialized[53]);
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                exchangeRecord.serialize(&char_serialized[53]);
                break;
            }
            default:
                break;
        }
    }

    void deserialize(const char* serialized, int total_size) {
        Method* method_serialized = (Method*) serialized;
        method = *method_serialized;
        method_serialized++;
        total_size -= sizeof(Method);
        INSTANCE_TYPE* type_serialized = (INSTANCE_TYPE*) method_serialized;
        instanceType = *type_serialized;
        type_serialized++;
        total_size -= sizeof(INSTANCE_TYPE);
        bool* bool_serialized = (bool*) type_serialized;
        closeConnection = *bool_serialized;
        bool_serialized++;
        total_size -= sizeof(bool);
        char* char_serialized = (char*) bool_serialized;
	    for (int i = 0; i < 50; i++) {
            responseFifoPath[i] = char_serialized[i];
        }
        for (int i = 0; i < 3; i++) {
            code[i] = char_serialized[50 + i];
        }
        total_size -= 3 * sizeof(char);
        switch(instanceType) {
            case WEATHER_MICROSERVICE: {
                weatherRecord.deserialize(&char_serialized[53], total_size);
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                exchangeRecord.deserialize(&char_serialized[53], total_size);
                break;
            }
            default:
                break;
        }
    }

    size_t get_bytes_size() {
        size_t common_size = sizeof(Method) + sizeof(INSTANCE_TYPE) + sizeof(char) * 50 +
                             sizeof(char) * 3 + sizeof(bool);
        size_t instance_size;
        switch(instanceType) {
            case WEATHER_MICROSERVICE: {
                instance_size = weatherRecord.get_bytes_size();
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                instance_size = exchangeRecord.get_bytes_size();
                break;
            }
            default:
                instance_size = 0;
                break;
        }
        return common_size + instance_size;
    }

    void serialize_with_size(char* serialized, size_t size) {
        int* int_serialized = (int*) serialized;
        *int_serialized = static_cast<int>(size - sizeof(int));
        int_serialized++;
        char* request_serialized = (char*) int_serialized;
        serialize(request_serialized);
    }
};

struct ConnectionRequest {
    char senderResponseFifoPath[50];
    INSTANCE_TYPE instanceType;

    void serialize(char* serialized) {
        INSTANCE_TYPE* enum_serialized = (INSTANCE_TYPE*) serialized;
        *enum_serialized = instanceType;
        enum_serialized++;
        char* char_serialized = (char*) enum_serialized;
	    strcpy(char_serialized, senderResponseFifoPath);
    }

    void deserialize(char* serialized, int total_size) {
        INSTANCE_TYPE* enum_serialized = (INSTANCE_TYPE*) serialized;
        instanceType = *enum_serialized;
        enum_serialized++;
        char* char_serialized = (char*) enum_serialized;
        for (int i = 0; i < 50; i++) {
            senderResponseFifoPath[i] = char_serialized[i];
        }
    }

    size_t get_bytes_size() {
        return sizeof(INSTANCE_TYPE) + sizeof(char) * 50;
    }

    void serialize_with_size(char* serialized, size_t size) {
        int* int_serialized = (int*) serialized;
        *int_serialized = static_cast<int>(size - sizeof(int));
        int_serialized++;
        char* request_serialized = (char*) int_serialized;
        serialize(request_serialized);
    }
};


#endif //PRIMER_PROYECTO_MESSAGE_H
