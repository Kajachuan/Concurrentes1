#include <cstring>
#include "ClientController.h"
#include "../logger/Logger.h"

using namespace std;

int main(int argc, char *argv[]) {
    auto logger = Logger::getInstance("client main");
    Logger::setLogLevel(DEBUG);
    ClientController client("/tmp/register-portal", "/tmp/portal-client");
    std::string command;

    while (command!= "end") {
        std::cout << "Insert your command here or end to finish: ";
        std::cin >> command;
        while (command.size() != 5 and command != "end") {
            logger->logMessage(ERROR, "Command must be 5 chars long: " + command);
            std::cout << "Command must be 5 chars long. Insert your new command: ";
            std::cin >> command;
        }
        logger->logMessage(DEBUG, "Recieved command: " + command);
        if (command!= "end") {
            MSRequest message{};
            if (CHAR_TO_SERVICE.count(command.at(0)) == 1 and CHAR_TO_METHOD.count(command.at(1)) == 1) {
                message.instanceType = CHAR_TO_SERVICE.at(command.at(0));
                message.method = CHAR_TO_METHOD.at(command.at(1));
                strcpy(message.code, command.substr(2, 3).c_str());
                std::cout << client.portal_request(message) << std::endl;
            } else {

                std::cout << "Invalid command w for weather, c for exchange, r for read, w for write" << std::endl;
            }
        }
    }
    Logger::endLogger();
}
