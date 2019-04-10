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
}

void PortalController::endPortal(){
    logger->logMessage(DEBUG, "Waiting child processes and clients to end connections");
    int status;
    for (int i = 0; i < forkedChilds; ++i) {
        wait(&status);
    }
    connectionRequestFifo->deleteFifo();

    logger->logMessage(DEBUG, "Sending close connection to services");
    MSRequest requestMessage{};
    requestMessage.closeConnection = true;
    std::map<INSTANCE_TYPE, std::map<std::string, FifoWriter *>>::iterator serviceIterator;
    for (serviceIterator = servicesRequestFifos.begin(); serviceIterator != servicesRequestFifos.end();
         serviceIterator++) {
        std::map<std::string, FifoWriter *>::iterator instanceIterator;
        for (instanceIterator = serviceIterator->second.begin(); instanceIterator != serviceIterator->second.end();
             instanceIterator++) {
            instanceIterator->second->write_fifo(static_cast<void *>(&requestMessage), sizeof(MSRequest));
            delete instanceIterator->second;
        }
    }
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
                                                        servicesResponseFifoPath, getRequestServicesMap());
                    while (!msQueryController.process_requests()) {};
                    return false;
                } else {
                    forkedChilds++;
                }
            }
            case EXCHANGE_MICROSERVICE:
            case WEATHER_MICROSERVICE: {
                logger->logMessage(DEBUG, "Service: " + std::string(serviceNames[requestMessage.instanceType]) +
                " registration request in fifo path: " + std::string(requestMessage.senderResponseFifoPath));
                if (servicesRequestFifos[requestMessage.instanceType].count(
                        requestMessage.senderResponseFifoPath) == 0) {
                    logger->logMessage(DEBUG, "Creating new service request fifo: " +
                    std::string(requestMessage.senderResponseFifoPath));
                    FifoWriter *servFifo = new FifoWriter(requestMessage.senderResponseFifoPath);
                    servFifo->open_fifo();
                    servicesRequestFifos[requestMessage.instanceType][requestMessage.senderResponseFifoPath] = servFifo;
                }
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

std::map<INSTANCE_TYPE, FifoWriter*> PortalController::getRequestServicesMap() {
    std::map<INSTANCE_TYPE, FifoWriter*> resultServiceMap;
    std::map<INSTANCE_TYPE, std::map<std::string, FifoWriter *>>::iterator serviceIterator;
    for (serviceIterator = servicesRequestFifos.begin(); serviceIterator != servicesRequestFifos.end();
    serviceIterator++) {
        // Here we can make some load balancing stuff
        // Now is getting the first service of every type
        resultServiceMap[serviceIterator->first] = (serviceIterator->second).begin()->second;
    }
    return resultServiceMap;
}