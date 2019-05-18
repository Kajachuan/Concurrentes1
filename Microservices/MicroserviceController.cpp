#include <cstring>
#include "MicroserviceController.h"
#include "../Commons/Messages.h"

LoggerClient MicroserviceController::logger = LoggerClient("MicroserviceController");

MicroserviceController::MicroserviceController(std::string registerRequestFifoPathName, std::string requestFifoPathName,
                                               INSTANCE_TYPE instanceType) {
    logger.logMessage(DEBUG, "Connecting to the fifo to register MS to portal: "+ registerRequestFifoPathName);
    FifoWriter registerRequestFifo(registerRequestFifoPathName);
    registerRequestFifo.open_fifo();
    ConnectionRequest connectionRequest{"", instanceType};
    std::strcpy(connectionRequest.senderResponseFifoPath, requestFifoPathName.c_str());
    registerRequestFifo.write_fifo(static_cast<void *>(&connectionRequest), sizeof(ConnectionRequest));

    logger.logMessage(DEBUG, "Opening request microservice fifo with path: " + requestFifoPathName);
    requestFifo = new FifoReader(requestFifoPathName);
    requestFifo->open_fifo();
}

MicroserviceController::~MicroserviceController() {
    requestFifo->deleteFifo();
    delete requestFifo;
}
