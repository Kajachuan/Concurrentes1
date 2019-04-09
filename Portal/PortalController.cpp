#include <iostream>
#include <cstring>
#include "PortalController.h"
#include "../logger/Logger.h"

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

    logger->logMessage(DEBUG, "Connecting to the fifo that writes messages to the client in path: " + responseFifoPath);
    responseFifo = new FifoWriter(responseFifoPath);
    responseFifo->open_fifo();
    logger->logMessage(DEBUG, "Connecting to the fifo that read messages from the client in path: " + requestFifoPath);
    requestFifo = new FifoReader(requestFifoPath);
    requestFifo->open_fifo();
}

PortalController::~PortalController(){
    delete responseFifo;
    delete requestFifo;
}

void PortalController::process_requests() {
    MSRequest request_message;
    ssize_t readBytes = requestFifo->read_fifo(static_cast<void *>(&request_message), sizeof(request_message));
    if (readBytes > 0) {
        std::string clientRequest = "Service: ";
        logger->logMessage(DEBUG, request_message.asString());
        // ... Aca voy a pedir las cosas a los ms y despues lo devuelvo como se debe
        PortalResponse msResponse = getMSResponse(request_message);
        logger->logMessage(DEBUG, "Writing response to client");
        responseFifo->write_fifo(static_cast<const void *>(&msResponse), sizeof(PortalResponse));
    }
}

PortalResponse PortalController::getMSResponse(MSRequest requestMessage) {
    std::string responseMSFifoPath = "/tmp/testResponseFifo";
    strcpy(requestMessage.responseFifoPath, responseMSFifoPath.c_str());
    requestMessage.closeConnection = false;
    logger->logMessage(DEBUG, "Sending response fifo path to ms and request: " + requestMessage.asString());
    requestMSFifo->write_fifo(static_cast<void *>(&requestMessage), sizeof(MSRequest));

    logger->logMessage(DEBUG, "Opening response fifo for ms with path: " + responseMSFifoPath);
    FifoReader responseMSFifo(responseMSFifoPath);
    responseMSFifo.open_fifo();
    logger->logMessage(DEBUG, "Reading response fifo from ms");
    PortalResponse responseMessage{};
    ssize_t readedBytes = responseMSFifo.read_fifo(static_cast<void *>(&responseMessage),
            sizeof(PortalResponse));
    if (readedBytes > 0) {
        logger->logMessage(DEBUG, "Received response from ms: " + responseMessage.asString());
    }
    return responseMessage;
}
