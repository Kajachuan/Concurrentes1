#include <iostream>
#include "PortalController.h"

const int BUFFER_SIZE = 100;

PortalController::PortalController(const std::string output_fifo_path_name,
                                   const std::string input_fifo_path_name) {
    output = new FifoWriter(output_fifo_path_name);
    output->open_fifo();
    input = new FifoReader(input_fifo_path_name);
    input->open_fifo();
}

PortalController::~PortalController() = default;

void PortalController::process_requests() {
    char buffer[BUFFER_SIZE];
    ssize_t readedBytes = input->read_fifo(static_cast<void *>(buffer), BUFFER_SIZE);
    if (readedBytes > 0) {
        std::string request_message = buffer;
        request_message.resize(static_cast<unsigned long>(readedBytes));
        std::cout << request_message << std::endl;
        std::string response_message = "asd";
        output->write_fifo(static_cast<const void *>(response_message.c_str()), response_message.length());
    }
}
