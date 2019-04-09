#include <iostream>
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
    struct portal_request_message_t request_message;
    ssize_t readBytes = requestFifo->read_fifo(static_cast<void *>(&request_message), sizeof(request_message));
    if (readBytes > 0) {
        std::string clientRequest = "Service: ";
        clientRequest = clientRequest + request_message.service + "; Method: " + request_message.method + "; Code: "
                + request_message.code;
        logger->logMessage(DEBUG, clientRequest);
        // ... Aca voy a pedir las cosas a los ms y despues lo devuelvo como se debe
        getMSResponse();
        struct portal_response_message_t response_message;
        response_message.service = request_message.service;
        if (request_message.service == 'C') {
            response_message.exchange_rate = 44.78;
        } else if (request_message.service == 'W') {
            response_message.temperature = 19;
            response_message.humidity = 98;
            response_message.pressure = 1030;
        }
        logger->logMessage(DEBUG, "Writing response to client");
        responseFifo->write_fifo(static_cast<const void *>(&response_message), sizeof(response_message));
    }
}

void PortalController::getMSResponse() {
    logger->logMessage(DEBUG, "Sending response fifo path to ms");
    std::string responseMSFifoPath = "/tmp/testResponseFifo";
    requestMSFifo->write_fifo(responseMSFifoPath.c_str(), responseMSFifoPath.length());

    logger->logMessage(DEBUG, "Opening response fifo for ms with path: " + responseMSFifoPath);
    FifoReader responseMSFifo(responseMSFifoPath);
    responseMSFifo.open_fifo();
    logger->logMessage(DEBUG, "Reading response fifo from ms");
    char buffer[BUFFER_SIZE];
    ssize_t readedBytes = responseMSFifo.read_fifo(static_cast<void *>(buffer), BUFFER_SIZE);
    if (readedBytes > 0) {
        std::string responseMessage = buffer;
        responseMessage.resize(static_cast<unsigned long>(readedBytes));
        logger->logMessage(DEBUG, "Received response from ms: " + responseMessage);
        requestMSFifo = new FifoWriter(responseMessage);
    }
}