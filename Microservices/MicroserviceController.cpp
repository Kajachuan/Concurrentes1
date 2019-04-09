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
