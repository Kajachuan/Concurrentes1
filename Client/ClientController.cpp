#include <iostream>
#include "ClientController.h"

const int BUFFER_SIZE = 100;

ClientController::ClientController(const std::string output_fifo_path_name, const std::string input_fifo_path_name) {
    input = new FifoReader(input_fifo_path_name);
    input->open_fifo();
    output = new FifoWriter(output_fifo_path_name);
    output->open_fifo();
}

ClientController::~ClientController() = default;

std::string ClientController::portal_request(std::string request_message) {
    char buffer[BUFFER_SIZE];
    output->write_fifo(static_cast<const void *>(request_message.c_str()), request_message.length());
    ssize_t readedBytes = input->read_fifo(static_cast<void *>(buffer), BUFFER_SIZE);
    std::string response_message = buffer;
    response_message.resize(static_cast<unsigned long>(readedBytes));
    return response_message;
}