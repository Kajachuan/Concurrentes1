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
    registerRequestFifo.write_fifo(static_cast<void *>(&connectionRequest), sizeof(ConnectionRequest));

    logger->logMessage(DEBUG, "Connecting to the fifo that read responses from the portal: " + responseFifoPathName);
    responseFifo = new FifoReader(responseFifoPathName);
    responseFifo->open_fifo();

    ssize_t readBytes = responseFifo->read_fifo(static_cast<void *>(&connectionRequest), sizeof(connectionRequest));
    if (readBytes > 0 and connectionRequest.instanceType == MS_QUERY_CONTROLLER) {
        logger->logMessage(DEBUG, "Connecting to the fifo that writes messages from the portal: " +
        std::string(connectionRequest.senderResponseFifoPath));
        requestFifo = new FifoWriter(connectionRequest.senderResponseFifoPath);
        requestFifo->open_fifo();
    } else {
        logger->logMessage(ERROR, "No connection request from portal");
    }

}

ClientController::~ClientController() {
    delete responseFifo;
    delete requestFifo;
}

std::string ClientController::portal_request(std::string request_message) {
    logger->logMessage(INFO, "Sending client request to portal: " + request_message);

    MSRequest message{};
    message.instanceType = CHAR_TO_SERVICE.at(request_message.at(0));
    message.method = CHAR_TO_METHOD.at(request_message.at(1));
    strcpy(message.code, request_message.substr(2, 3).c_str());
    requestFifo->write_fifo(static_cast<const void *>(&message), sizeof(message));

    PortalResponse portalResponse{};
    ssize_t readedBytes = responseFifo->read_fifo(static_cast<void *>(&portalResponse),
            sizeof(PortalResponse));
    logger->logMessage(INFO, "Received response from the portal: " + portalResponse.asString());
    return portalResponse.asString();
}