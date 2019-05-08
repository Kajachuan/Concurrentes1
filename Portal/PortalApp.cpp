#include <iostream>
#include "PortalController.h"
#include "../IPC/Signal/SIGINTShutdownHandler.h"
#include "../IPC/Signal/SignalHandler.h"
#include "../Logger/LoggerClient.h"

int main(int argc, char const *argv[]) {
    auto logger = LoggerClient("portal main");
    char pidText[10];
    snprintf(pidText, sizeof(pidText), "%d", getpid());
    logger.logMessage(INFO, std::string("Start portal app with pid: ") + pidText);
    auto portal = PortalController("/tmp/register-portal");
    SIGINTShutdownHandler sigintShutdownHandler;
    SignalHandler::getInstance()->registerHandler(SIGINT, &sigintShutdownHandler);
    int processResult = 1;
    while (processResult and sigintShutdownHandler.getGracefulQuit() != 1) {
        processResult = portal.processConnectionRequests();
    }
    if (processResult < 0 and sigintShutdownHandler.getGracefulQuit() == 1) {
        portal.endPortal();
    }
}
