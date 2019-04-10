#include <iostream>
#include "PortalController.h"

int main(int argc, char const *argv[]) {
    auto logger = Logger::getInstance("portal main");
    Logger::setLogLevel(DEBUG);
    auto portal = PortalController("/tmp/register-portal");
    logger->logMessage(DEBUG, "Start processing");
    int processResult = portal.processConnectionRequests();
    while (processResult > 0) {
        processResult = portal.processConnectionRequests();
    };
    if (processResult < 0) {
        portal.endPortal();
    }
    Logger::endLogger();
}
