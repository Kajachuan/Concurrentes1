#include <cstring>
#include "ClientController.h"

using namespace std;

int main(int argc, char *argv[]) {
    auto logger = Logger::getInstance("client main");
    Logger::setLogLevel(DEBUG);
    ClientController client("/tmp/register-portal", "/tmp/portal-client");
    std::string command;

    while (command != "end") {
        std::cout << "Insert your command here (type 'help' for information) or 'end' to finish: ";
        std::cin >> command;
        while (command.size() != 5 && command != "end" && command != "help") {
            logger->logMessage(ERROR, "Command must be 5 chars long: " + command);
            std::cout << "Command must be 5 chars long. Insert your new command: ";
            std::cin >> command;
        }
        logger->logMessage(DEBUG, "Recieved command: " + command);
        if (command == "help") {
            std::cout << "The command is composed of five joined characters:" << std::endl;
            std::cout << "\t- The first character can be 'w' (Weather) or 'c' (Exchange)." << std::endl;
            std::cout << "\t- The second character can be 'r' (Read) or 'w' (Write)." << std::endl;
            std::cout << "\t- The remaining characters are the code of the city or the currency" << std::endl;
            std::cout << "\tExample: wrbas (Read weather information of Buenos Aires)" << std::endl;
        } else if (command != "end") {
            try {
                MSRequest message{};
                if (CHAR_TO_SERVICE.count(command.at(0)) == 1 and CHAR_TO_METHOD.count(command.at(1)) == 1) {
                    message.instanceType = CHAR_TO_SERVICE.at(command.at(0));
                    message.method = CHAR_TO_METHOD.at(command.at(1));
                    strcpy(message.code, command.substr(2, 3).c_str());

                    if (message.method == WRITE) {
                        std::string password;
                        std::cout << "Insert password: ";
                        std::cin >> password;
                        if (password != "admin") {
                            std::cout << "Wrong password" << std::endl;
                            continue;
                        }
                        if (message.instanceType == WEATHER_MICROSERVICE) {
                            std::string temperature, pressure, humidity, name;
                            while (true) {
                                try {
                                    std::cout << "Insert temperature: ";
                                    std::cin >> temperature;
                                    message.weatherRecord.temperature = stof(temperature);
                                    break;
                                }
                                catch (...) {
                                    std::cout << "Invalid value. Try again" << std::endl;
                                }
                            }
                            while (true) {
                                try {
                                    std::cout << "Insert pressure: ";
                                    std::cin >> pressure;
                                    message.weatherRecord.pressure = stof(pressure);
                                    break;
                                }
                                catch (...) {
                                    std::cout << "Invalid value. Try again" << std::endl;
                                }
                            }
                            while (true) {
                                try {
                                    std::cout << "Insert humidity: ";
                                    std::cin >> humidity;
                                    message.weatherRecord.humidity = stof(humidity);
                                    break;
                                }
                                catch (...) {
                                    std::cout << "Invalid value. Try again" << std::endl;
                                }
                            }
                            std::cout << "Insert name: ";
                            std::cin >> name;
                            std::strcpy(message.exchangeRecord.name, name.c_str());
                        } else {
                            std::string exchange, name;
                            while (true) {
                                try {
                                    std::cout << "Insert exchange: ";
                                    std::cin >> exchange;
                                    message.exchangeRecord.exchange = stof(exchange);
                                    break;
                                }
                                catch (...) {
                                    std::cout << "Invalid value. Try again" << std::endl;
                                }
                            }
                            std::cout << "Insert name: ";
                            std::cin >> name;
                            std::strcpy(message.exchangeRecord.name, name.c_str());
                        }
                    }
                    std::cout << message.asString() << std::endl;
                    std::cout << client.portal_request(message) << std::endl;
                } else {
                    std::cout << "Invalid command w for weather, c for exchange, r for read, w for write" << std::endl;
                }
            }

            catch (...) {
                logger->logMessage(ERROR, "Unknown command: " + command);
                std::cout << "Unknown command." << std::endl;
            }
        }
    }
    Logger::endLogger();
}
