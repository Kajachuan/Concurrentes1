#include <utility>

#include <iostream>
#include <cstring>
#include "MSQueryController.h"
#include "../logger/Logger.h"

Logger *MSQueryController::logger = Logger::getInstance("MSQueryController");

MSQueryController::MSQueryController(std::string clientRequestFifoPath, std::string clientResponseFifoPath,
        std::string servicesResponseFifoPath, std::map<INSTANCE_TYPE, FifoWriter*> servicesRequestFifos) {
    this->servicesRequestFifos = servicesRequestFifos;
    this->servicesResponseFifoPath = std::move(servicesResponseFifoPath);
    this->servicesResponseFifo = nullptr;

    logger->logMessage(DEBUG, "Connecting to the fifo that writes messages to the client in path: "
    + clientResponseFifoPath);
    clientResponseFifo = new FifoWriter(clientResponseFifoPath);
    clientResponseFifo->open_fifo();
    ConnectionRequest connectionRequest{"", MS_QUERY_CONTROLLER};
    std::strcpy(connectionRequest.senderResponseFifoPath, clientRequestFifoPath.c_str());
    size_t total_size = connectionRequest.get_bytes_size() + sizeof(int);
    char serialized_message[total_size];
    connectionRequest.serialize_with_size(serialized_message, total_size);
    clientResponseFifo->write_fifo(static_cast<const void *>(serialized_message), total_size);

    logger->logMessage(DEBUG, "Waiting for the client to connect " + clientRequestFifoPath);
    clientRequestFifo = new FifoReader(clientRequestFifoPath);
    clientRequestFifo->open_fifo();
}

MSQueryController::~MSQueryController(){
    delete clientResponseFifo;
    delete clientRequestFifo;
    delete servicesResponseFifo;
}

bool MSQueryController::process_requests() {
    MSRequest requestMessage{};
    int message_size;
    ssize_t readBytes = clientRequestFifo->read_fifo(static_cast<void *>(&message_size), sizeof(int));
    if (readBytes > 0) {
        char serialized[message_size];
        readBytes = clientRequestFifo->read_fifo(static_cast<void *>(serialized), static_cast<size_t>(message_size));
        if (readBytes > 0) {
            requestMessage.deserialize(serialized, message_size);
            if (!requestMessage.closeConnection) {
                logger->logMessage(DEBUG, "Read request message: " + requestMessage.asString());
                PortalResponse msResponse = getMSResponse(requestMessage);
                logger->logMessage(DEBUG, "Writing response to client: " + msResponse.asString());
                size_t total_size = msResponse.get_bytes_size() + sizeof(int);
                char serialized_message[total_size];
                msResponse.serialize_with_size(serialized_message, total_size);
                clientResponseFifo->write_fifo(static_cast<const void *>(serialized_message), total_size);
            }
        }
    }
    return requestMessage.closeConnection;
}

PortalResponse MSQueryController::getMSResponse(MSRequest requestMessage) {
    PortalResponse responseMessage{};
    if (servicesRequestFifos.count(requestMessage.instanceType) == 0) {
        logger->logMessage(WARNING, "No instance for this service");
        responseMessage.requestError = true;
    } else {
        strcpy(requestMessage.responseFifoPath, servicesResponseFifoPath.c_str());
        requestMessage.closeConnection = false;
        logger->logMessage(DEBUG, "Sending request to microservice: " + requestMessage.asString());
        size_t total_size = requestMessage.get_bytes_size() + sizeof(int);
        char serialized_message[total_size];
        requestMessage.serialize_with_size(serialized_message, total_size);
        servicesRequestFifos[requestMessage.instanceType]->write_fifo(static_cast<const void *>
                                                                     (serialized_message), total_size);

        if (servicesResponseFifo == nullptr) {
            logger->logMessage(DEBUG, "Opening response fifo for ms with path: " + servicesResponseFifoPath);
            servicesResponseFifo = new FifoReader(servicesResponseFifoPath);
            servicesResponseFifo->open_fifo();
        }

        logger->logMessage(DEBUG, "Reading response fifo from ms");
        int message_size;
        ssize_t readBytes = servicesResponseFifo->read_fifo(static_cast<void *>(&message_size), sizeof(int));
        if (readBytes > 0) {
            char serialized[message_size];
            readBytes = servicesResponseFifo->read_fifo(static_cast<void *>(serialized),
                                                        static_cast<size_t>(message_size));
            if (readBytes > 0) {
                requestMessage.deserialize(serialized, message_size);
                logger->logMessage(DEBUG, "Received response from ms: " + responseMessage.asString());
            }
        }
    }
    return responseMessage;
}


