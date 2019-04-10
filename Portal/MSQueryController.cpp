#include <utility>

#include <iostream>
#include <cstring>
#include "MSQueryController.h"
#include "../logger/Logger.h"

Logger *MSQueryController::logger = Logger::getInstance("MSQueryController");

MSQueryController::MSQueryController(std::string clientRequestFifoPath, std::string clientResponseFifoPath,
        std::string servicesResponseFifoPath, FifoWriter *servicesRequestFifos) {
    this->servicesRequestFifos = servicesRequestFifos;
    this->servicesResponseFifoPath = std::move(servicesResponseFifoPath);

    logger->logMessage(DEBUG, "Connecting to the fifo that writes messages to the client in path: "
    + clientResponseFifoPath);
    clientResponseFifo = new FifoWriter(clientResponseFifoPath);
    clientResponseFifo->open_fifo();
    ConnectionRequest connectionRequest{"", MS_QUERY_CONTROLLER};
    std::strcpy(connectionRequest.senderResponseFifoPath, clientRequestFifoPath.c_str());
    clientResponseFifo->write_fifo(static_cast<void *>(&connectionRequest), sizeof(ConnectionRequest));

    logger->logMessage(DEBUG, "Waiting for the client to connect " + clientRequestFifoPath);
    clientRequestFifo = new FifoReader(clientRequestFifoPath);
    clientRequestFifo->open_fifo();
}

MSQueryController::~MSQueryController(){
    logger->logMessage(DEBUG, "Sending close connection to services");
    MSRequest requestMessage{};
    requestMessage.closeConnection = false;
    servicesRequestFifos->write_fifo(static_cast<void *>(&requestMessage), sizeof(MSRequest));
    delete clientResponseFifo;
    delete clientRequestFifo;
    delete servicesRequestFifos;
    delete servicesResponseFifo;
}

bool MSQueryController::process_requests() {
    MSRequest requestMessage{};
    ssize_t readBytes = clientRequestFifo->read_fifo(static_cast<void *>(&requestMessage), sizeof(MSRequest));
    if (readBytes > 0 and !requestMessage.closeConnection) {
        logger->logMessage(DEBUG, "Read request message: " + requestMessage.asString());
        PortalResponse msResponse = getMSResponse(requestMessage);
        logger->logMessage(DEBUG, "Writing response to client");
        clientResponseFifo->write_fifo(static_cast<const void *>(&msResponse), sizeof(PortalResponse));
    }
    return requestMessage.closeConnection;
}

PortalResponse MSQueryController::getMSResponse(MSRequest requestMessage) {
    strcpy(requestMessage.responseFifoPath, servicesResponseFifoPath.c_str());
    requestMessage.closeConnection = false;
    logger->logMessage(DEBUG, "Sending request to microservice: " + requestMessage.asString());
    servicesRequestFifos->write_fifo(static_cast<void *>(&requestMessage), sizeof(MSRequest));

    if (!servicesResponseFifoPath.empty()) {
        logger->logMessage(DEBUG, "Opening response fifo for ms with path: " + servicesResponseFifoPath);
        servicesResponseFifo = new FifoReader(servicesResponseFifoPath);
        servicesResponseFifo->open_fifo();
        servicesResponseFifoPath = "";
    }
    logger->logMessage(DEBUG, "Reading response fifo from ms");
    PortalResponse responseMessage{};
    ssize_t readedBytes = servicesResponseFifo->read_fifo(static_cast<void *>(&responseMessage),
            sizeof(PortalResponse));
    if (readedBytes > 0) {
        logger->logMessage(DEBUG, "Received response from ms: " + responseMessage.asString());
    }
    return responseMessage;
}


