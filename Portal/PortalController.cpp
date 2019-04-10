#include <iostream>
#include <cstring>
#include <wait.h>
#include "PortalController.h"
#include "../logger/Logger.h"
#include "MSQueryController.h"

Logger *PortalController::logger = Logger::getInstance("PortalController");

PortalController::PortalController(std::string connectionRequestFifoPath) {
    logger->logMessage(DEBUG, "Waiting to first register in fifo path: " + connectionRequestFifoPath);
    connectionRequestFifo = new FifoReader(connectionRequestFifoPath);
    connectionRequestFifo->open_fifo();
    forkedChilds = 0;
}

PortalController::~PortalController(){
    delete connectionRequestFifo;
    delete servicesRequestFifos;
}

void PortalController::endPortal(){
    logger->logMessage(DEBUG, "Waiting child processes");
    int status;
    for (int i = 0; i < forkedChilds; ++i) {
        wait(&status);
    }
    connectionRequestFifo->deleteFifo();

    logger->logMessage(DEBUG, "Sending close connection to services");
    MSRequest requestMessage{};
    requestMessage.closeConnection = true;
    servicesRequestFifos->write_fifo(static_cast<void *>(&requestMessage), sizeof(MSRequest));
}

int PortalController::processConnectionRequests() {
    ConnectionRequest requestMessage{};
    ssize_t readBytes = connectionRequestFifo->read_fifo(static_cast<void *>(&requestMessage), sizeof(requestMessage));
    if (readBytes > 0) {
        switch(requestMessage.instanceType) {
            case CLIENT: {
                logger->logMessage(DEBUG, "MSQueryController request, forking...");
                pid_t pid = fork();
                if (pid == 0) {
                    std::string clientRequestFifoPath = "/tmp/client-query" + std::to_string(forkedChilds);
                    std::string servicesResponseFifoPath = "/tmp/client-query" + std::to_string(forkedChilds);
                    MSQueryController msQueryController(clientRequestFifoPath, requestMessage.senderResponseFifoPath,
                                                        servicesResponseFifoPath, servicesRequestFifos);
                    while (!msQueryController.process_requests()) {};
                } else {
                    forkedChilds++;
                }
                return pid;
            }
            case WEATHER_MICROSERVICE: {
                logger->logMessage(DEBUG, "WEATHER_MICROSERVICE registration request in fifo path: " +
                        std::string(requestMessage.senderResponseFifoPath));
                servicesRequestFifos = new FifoWriter(requestMessage.senderResponseFifoPath);
                servicesRequestFifos->open_fifo();
                break;
            }
            case EXCHANGE_MICROSERVICE: {
                logger->logMessage(DEBUG, "EXCHANGE_MICROSERVICE registration request");
                break;
            }
            default: {
                logger->logMessage(DEBUG, "Wrong message, instanceType: " +
                std::string(serviceNames[requestMessage.instanceType]));
                break;
            }
        }
    }
    return true;
}