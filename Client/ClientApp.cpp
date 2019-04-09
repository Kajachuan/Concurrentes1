#include <cstring>
#include "ClientController.h"
#include "../logger/Logger.h"

using namespace std;

int main(int argc, char *argv[]) {
    std::cout << "Insert your command here: ";
    std::string command;
    std::cin >> command;
    while (command.size() != 5) {
        std::cout << "Command must be 5 chars long. Insert your new command: ";
        std::cin >> command;
    }
    auto logger = Logger::getInstance("client main");
    Logger::setLogLevel(DEBUG);
    logger->logMessage(DEBUG, "test");
    auto *client = new ClientController("/tmp/client-portal", "/tmp/portal-client");
    std::cout << client->portal_request(command) << std::endl;
    Logger::endLogger();
}
