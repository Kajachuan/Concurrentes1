#include <iostream>
#include <cstring>
#include <sstream>
#include "ClientController.h"

const int BUFFER_SIZE = 100;
Logger *ClientController::logger = Logger::getInstance("ClientController");

ClientController::ClientController(const std::string registerRequestFifoPathName,
        const std::string responseFifoPathName) {
    logger->logMessage(DEBUG, "Connecting to the fifo to register client to portal: "+ registerRequestFifoPathName);
    FifoWriter registerRequestFifo(registerRequestFifoPathName);
    registerRequestFifo.open_fifo();
    ConnectionRequest connectionRequest{"", CLIENT};
    std::strcpy(connectionRequest.senderResponseFifoPath, responseFifoPathName.c_str());
    size_t total_size = connectionRequest.get_bytes_size() + sizeof(int);
    char serialized_message[total_size];
    connectionRequest.serialize_with_size(serialized_message, total_size);
    registerRequestFifo.write_fifo(static_cast<const void *>(serialized_message), total_size);

    logger->logMessage(DEBUG, "Connecting to the fifo that read responses from the portal: " + responseFifoPathName);
    responseFifo = new FifoReader(responseFifoPathName);
    responseFifo->open_fifo();

    int message_size;
    ssize_t readBytes = responseFifo->read_fifo(static_cast<void *>(&message_size), sizeof(int));
    if (readBytes > 0) {
        char serialized[message_size];
        readBytes = responseFifo->read_fifo(static_cast<void *>(serialized), static_cast<size_t>(message_size));
        if (readBytes > 0) {
            connectionRequest.deserialize(serialized, message_size);
            if (connectionRequest.instanceType == MS_QUERY_CONTROLLER) {
                logger->logMessage(DEBUG, "Connecting to the fifo that writes messages to the portal: " +
                                          std::string(connectionRequest.senderResponseFifoPath));
                requestFifo = new FifoWriter(connectionRequest.senderResponseFifoPath);
                requestFifo->open_fifo();
            } else {
                logger->logMessage(ERROR, "No connection request from portal");
            }
        } else {
            logger->logMessage(ERROR, "No connection request from portal");
        }
    }
}

ClientController::~ClientController() {
    MSRequest requestMessage{};
    requestMessage.closeConnection = true;
    size_t total_size = requestMessage.get_bytes_size() + sizeof(int);
    char serialized_message[total_size];
    requestMessage.serialize_with_size(serialized_message, total_size);
    requestFifo->write_fifo(static_cast<const void *>(serialized_message), total_size);
    delete responseFifo;
    delete requestFifo;
}

std::string ClientController::portal_request(MSRequest requestMessage) {
    logger->logMessage(INFO, "Sending client request to portal: " + requestMessage.asString());
    size_t total_size = requestMessage.get_bytes_size() + sizeof(int);
    char serialized_message[total_size];
    requestMessage.serialize_with_size(serialized_message, total_size);
    requestFifo->write_fifo(static_cast<const void *>(serialized_message), total_size);

    PortalResponse portalResponse{};
    int message_size;
    ssize_t readBytes = responseFifo->read_fifo(static_cast<void *>(&message_size), sizeof(int));
    if (readBytes > 0) {
        char serialized[message_size];
        readBytes = responseFifo->read_fifo(static_cast<void *>(serialized), static_cast<size_t>(message_size));
        if (readBytes > 0) {
            portalResponse.deserialize(serialized, message_size);
            logger->logMessage(INFO, "Received response from the portal: " + portalResponse.asString());
        } else {
            logger->logMessage(ERROR, "No response from portal");
        }
    }
    return portalResponse.asString();
}