#include <iostream>
#include "../IPC/Signal/SIGINTShutdownHandler.h"
#include "../IPC/Signal/SignalHandler.h"
#include "../Logger/LoggerServer.h"

int main(int argc, char const *argv[]) {
    auto logger = LoggerServer::getInstance();
    LoggerServer::setLogLevel(DEBUG);
    SIGINTShutdownHandler sigintShutdownHandler;
    SignalHandler::getInstance()->registerHandler(SIGINT, &sigintShutdownHandler);
    std::cout << "Logger server started with pid: " << getpid() << std::endl;
    while (sigintShutdownHandler.getGracefulQuit() != 1) {
        logger->readNewLogs();
    }
    std::cout << "Logger server ended" << std::endl;
    LoggerServer::endLogger();
}
