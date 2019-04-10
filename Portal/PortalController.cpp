#include <iostream>
#include <cstring>
#include "PortalController.h"
#include "../logger/Logger.h"
#include "MSQueryController.h"

Logger *PortalController::logger = Logger::getInstance("PortalController");

PortalController::PortalController(std::string connectionRequestFifoPath) {
    logger->logMessage(DEBUG, "Waiting to first register in fifo path: " + connectionRequestFifoPath);
    connectionRequestFifo = new FifoReader(connectionRequestFifoPath);
    connectionRequestFifo->open_fifo();
}

PortalController::~PortalController(){
    connectionRequestFifo->deleteFifo();
    delete connectionRequestFifo;
    delete servicesRequestFifos;
}

bool PortalController::processConnectionRequests() {
    ConnectionRequest requestMessage{};
    ssize_t readBytes = connectionRequestFifo->read_fifo(static_cast<void *>(&requestMessage), sizeof(requestMessage));
    if (readBytes > 0) {
        switch(requestMessage.instanceType) {
            case CLIENT: {
                logger->logMessage(DEBUG, "MSQueryController request");
                MSQueryController msQueryController("/tmp/client-query", requestMessage.senderResponseFifoPath,
                                                    "/tmp/ms-query", servicesRequestFifos);
                while (!msQueryController.process_requests()) {};
                return false;
                break;
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