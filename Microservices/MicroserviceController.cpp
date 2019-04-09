#include "MicroserviceController.h"

Logger *MicroserviceController::logger = Logger::getInstance("MicroserviceController");

MicroserviceController::MicroserviceController(std::string output_fifo_path_name, std::string input_fifo_path_name) {
    logger->logMessage(DEBUG, "Registering to portal with fifo path: " + output_fifo_path_name);
    FifoWriter registerFifo(output_fifo_path_name);
    registerFifo.open_fifo();
    registerFifo.write_fifo(static_cast<const void *>(input_fifo_path_name.c_str()), input_fifo_path_name.length());

    logger->logMessage(DEBUG, "Opening request fifo with path: " + input_fifo_path_name);
    requestFifo = new FifoReader(input_fifo_path_name);
    requestFifo->open_fifo();
}

MicroserviceController::~MicroserviceController() = default;

void MicroserviceController::processRequest() {
    logger->logMessage(DEBUG, "Reading request fifo");
    char buffer[BUFFER_SIZE];
    ssize_t readedBytes = requestFifo->read_fifo(static_cast<void *>(buffer), BUFFER_SIZE);
    if (readedBytes > 0) {
        std::string request_message = buffer;
        request_message.resize(static_cast<unsigned long>(readedBytes));
        logger->logMessage(DEBUG, "Received request: " + request_message);
        if (responseFifos.count(request_message) == 0) {
            responseFifos[request_message] = new FifoWriter(request_message);
            responseFifos[request_message]->open_fifo();
        }

        std::string response_message = "msasd";
        logger->logMessage(DEBUG, "Sending response request: " + response_message);
        responseFifos[request_message]->write_fifo(static_cast<const void *>(response_message.c_str()),
                response_message.length());
    }
}
