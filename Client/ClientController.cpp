#include <iostream>
#include <cstring>
#include "ClientController.h"

const int BUFFER_SIZE = 100;
Logger *ClientController::logger = Logger::getInstance("ClientController");

ClientController::ClientController(const std::string output_fifo_path_name, const std::string input_fifo_path_name) {
    logger->logMessage(DEBUG, "Connecting to the fifo that read messages from the portal");
    input = new FifoReader(input_fifo_path_name);
    input->open_fifo();
    logger->logMessage(DEBUG, "Connecting to the fifo that writes messages from the portal");
    output = new FifoWriter(output_fifo_path_name);
    output->open_fifo();
}

ClientController::~ClientController() {
    delete input;
    delete output;
}

std::string ClientController::portal_request(std::string request_message) {
    logger->logMessage(INFO, "Sending client request to portal: " + request_message);
    output->write_fifo(static_cast<const void *>(request_message.c_str()), request_message.length());

    char buffer[BUFFER_SIZE];
    struct message_t message;
    message.service = request_message.at(0);
    message.method = request_message.at(1);
    strcpy(message.code, request_message.substr(2, 3).c_str());
    output->write_fifo(static_cast<const void *>(&message), sizeof(message));
    ssize_t readedBytes = input->read_fifo(static_cast<void *>(buffer), BUFFER_SIZE);
    std::string response_message = buffer;
    response_message.resize(static_cast<unsigned long>(readedBytes));
    logger->logMessage(INFO, "Received response from the portal: " + response_message);

    return response_message;
}