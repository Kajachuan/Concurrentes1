#include <iostream>
#include <cstring>
#include "PortalController.h"
#include "../logger/Logger.h"
#include "MSQueryController.h"

const int BUFFER_SIZE = 100;
Logger *PortalController::logger = Logger::getInstance("PortalController");

PortalController::PortalController(std::string responseFifoPath, std::string requestFifoPath,
                                   std::string registerMSFifoPath) {
    logger->logMessage(DEBUG, "Waiting to register MS in fifo path: " + registerMSFifoPath);
    FifoReader registerMSFifo(registerMSFifoPath);
    registerMSFifo.open_fifo();
    char buffer[BUFFER_SIZE];
    ssize_t readedBytes = registerMSFifo.read_fifo(static_cast<void *>(buffer), BUFFER_SIZE);
    if (readedBytes > 0) {
        std::string requestMSFifoPath = buffer;
        requestMSFifoPath.resize(static_cast<unsigned long>(readedBytes));
        logger->logMessage(DEBUG, "Storing request MS fifo in path: " + requestMSFifoPath);
        requestMSFifo = new FifoWriter(requestMSFifoPath);
        requestMSFifo->open_fifo();
    }

    logger->logMessage(DEBUG, "Connecting to the fifo that read request messages in path: "
    + requestFifoPath);
    requestFifo = new FifoReader(requestFifoPath);
    requestFifo->open_fifo();
}

PortalController::~PortalController(){
    delete requestFifo;
}

void PortalController::process_requests() {
    ConnectionRequest requestMessage{};
    ssize_t readBytes = requestFifo->read_fifo(static_cast<void *>(&requestMessage), sizeof(requestMessage));
    if (readBytes > 0) {
        switch(requestMessage.instanceType) {
            case CLIENT: {
                logger->logMessage(DEBUG, "MSQueryController request");
                MSQueryController msQueryController("/tmp/client-query", requestMessage.senderResponseFifoPath,
                                                    "/tmp/query-ms", requestMSFifo);
                msQueryController.process_requests();
                break;
            }
            case WEATHER_MICROSERVICE: {
                logger->logMessage(DEBUG, "WEATHER_MICROSERVICE registration request");
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
}