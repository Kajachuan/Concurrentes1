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

PortalController::~PortalController(){
    delete output;
    delete input;
}

void PortalController::process_requests() {
    struct message_t message;
    ssize_t readBytes = input->read_fifo(static_cast<void *>(&message), sizeof(message));
    if (readBytes > 0) {
        std::cout << "Service: " << message.service << "; Method: " << message.method << "; Code"
                  << message.code << std::endl;
        std::string response_message = "Mensaje recibido... Esperando respuesta";
        output->write_fifo(static_cast<const void *>(response_message.c_str()), response_message.length());
    }
}
