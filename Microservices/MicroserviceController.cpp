#include <cstring>
#include "MicroserviceController.h"
#include "../Commons/Messages.h"

Logger *MicroserviceController::logger = Logger::getInstance("MicroserviceController");

MicroserviceController::MicroserviceController(std::string registerRequestFifoPathName, std::string requestFifoPathName,
                                               INSTANCE_TYPE instanceType) {
    logger->logMessage(DEBUG, "Connecting to the fifo to register MS to portal: "+ registerRequestFifoPathName);
    FifoWriter registerRequestFifo(registerRequestFifoPathName);
    registerRequestFifo.open_fifo();
    ConnectionRequest connectionRequest{"", instanceType};
    std::strcpy(connectionRequest.senderResponseFifoPath, requestFifoPathName.c_str());
    size_t total_size = connectionRequest.get_bytes_size() + sizeof(int);
    char serialized_message[total_size];
    connectionRequest.serialize_with_size(serialized_message, total_size);
    registerRequestFifo.write_fifo(static_cast<const void *>(serialized_message), total_size);

    logger->logMessage(DEBUG, "Opening request microservice fifo with path: " + requestFifoPathName);
    requestFifo = new FifoReader(requestFifoPathName);
    requestFifo->open_fifo();
}

MicroserviceController::~MicroserviceController() {
    delete requestFifo;
}
