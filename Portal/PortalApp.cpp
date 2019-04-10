#include <iostream>
#include "PortalController.h"

int main(int argc, char const *argv[]) {
    auto logger = Logger::getInstance("portal main");
    Logger::setLogLevel(DEBUG);
    auto portal = PortalController("/tmp/register-portal");
    logger->logMessage(DEBUG, "Start processing");
    while (portal.processConnectionRequests()) {};
    Logger::endLogger();
}
