#include <cstring>
#include "ClientController.h"
#include "../logger/Logger.h"

using namespace std;

int main(int argc, char *argv[]) {
    auto logger = Logger::getInstance("client main");
    Logger::setLogLevel(DEBUG);
    std::cout << "Insert your command here: ";
    std::string command;
    std::cin >> command;
    logger->logMessage(DEBUG, "Recieved command: " + command);
    while (command.size() != 5) {
        logger->logMessage(ERROR, "Command must be 5 chars long");
        std::cout << "Command must be 5 chars long. Insert your new command: ";
        std::cin >> command;
        logger->logMessage(DEBUG, "Recieved command: " + command);
    }
    auto *client = new ClientController("/tmp/client-portal", "/tmp/portal-client");
    std::cout << client->portal_request(command) << std::endl;
    Logger::endLogger();
}
