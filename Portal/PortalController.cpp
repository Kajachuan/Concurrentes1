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
    struct portal_request_message_t request_message;
    ssize_t readBytes = input->read_fifo(static_cast<void *>(&request_message), sizeof(request_message));
    if (readBytes > 0) {
        std::cout << "Service: " << request_message.service << "; Method: " << request_message.method
                  << "; Code: " << request_message.code << std::endl;
        // ... Aca voy a pedir las cosas a los ms y despues lo devuelvo como se debe
        struct portal_response_message_t response_message;
        response_message.service = request_message.service;
        if (request_message.service == 'C') {
            response_message.exchange_rate = 44.78;
        } else if (request_message.service == 'W') {
            response_message.temperature = 19;
            response_message.humidity = 98;
            response_message.pressure = 1030;
        }
        output->write_fifo(static_cast<const void *>(&response_message), sizeof(response_message));
    }
}
