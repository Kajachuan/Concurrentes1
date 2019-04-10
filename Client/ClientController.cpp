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
    logger->logMessage(DEBUG, "openinn "+ registerRequestFifoPathName);
    registerRequestFifo.open_fifo();
    ConnectionRequest connectionRequest{"", CLIENT};
    std::strcpy(connectionRequest.senderResponseFifoPath, responseFifoPathName.c_str());
    registerRequestFifo.write_fifo(static_cast<void *>(&connectionRequest), sizeof(ConnectionRequest));

    logger->logMessage(DEBUG, "Connecting to the fifo that read responses from the portal: " + responseFifoPathName);
    responseFifo = new FifoReader(responseFifoPathName);
    responseFifo->open_fifo();

    ssize_t readBytes = responseFifo->read_fifo(static_cast<void *>(&connectionRequest), sizeof(connectionRequest));
    if (readBytes > 0 and connectionRequest.instanceType == MS_QUERY_CONTROLLER) {
        logger->logMessage(DEBUG, "Connecting to the fifo that writes messages to the portal: " +
        std::string(connectionRequest.senderResponseFifoPath));
        requestFifo = new FifoWriter(connectionRequest.senderResponseFifoPath);
        requestFifo->open_fifo();
    } else {
        logger->logMessage(ERROR, "No connection request from portal");
    }

}

ClientController::~ClientController() {
    MSRequest requestMessage{};
    requestMessage.closeConnection = true;
    requestFifo->write_fifo(static_cast<const void *>(&requestMessage), sizeof(requestMessage));
    delete responseFifo;
    delete requestFifo;
}

std::string ClientController::portal_request(MSRequest requestMessage) {
    logger->logMessage(INFO, "Sending client request to portal: " + requestMessage.asString());
    requestFifo->write_fifo(static_cast<const void *>(&requestMessage), sizeof(requestMessage));

    PortalResponse portalResponse{};
    ssize_t readedBytes = responseFifo->read_fifo(static_cast<void *>(&portalResponse),
            sizeof(PortalResponse));
    if (readedBytes > 0) {
        logger->logMessage(INFO, "Received response from the portal: " + portalResponse.asString());
    } else {
        logger->logMessage(ERROR, "No response from portal");
    }
    return portalResponse.asString();
}