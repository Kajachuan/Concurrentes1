#include <iostream>
#include "PortalController.h"
#include "../IPC/Signal/SIGINTShutdownHandler.h"
#include "../IPC/Signal/SignalHandler.h"
#include "../Logger/Logger.h"

int main(int argc, char const *argv[]) {
    auto logger = Logger::getInstance("portal main");
    Logger::setLogLevel(DEBUG);
    auto portal = PortalController("/tmp/register-portal");
    logger->logMessage(DEBUG, "Start processing");
    SIGINTShutdownHandler sigintShutdownHandler;
    SignalHandler::getInstance()->registerHandler(SIGINT, &sigintShutdownHandler);
    int processResult = 1;
    while (processResult and sigintShutdownHandler.getGracefulQuit() != 1) {
        processResult = portal.processConnectionRequests();
    }
    if (processResult < 0 and sigintShutdownHandler.getGracefulQuit() == 1) {
        portal.endPortal();
    }
    Logger::endLogger();
}
