#include <iostream>
#include <cstring>
#include <sstream>
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

    struct portal_request_message_t message;
    message.service = CHAR_TO_SERVICE.at(request_message.at(0));
    message.method = CHAR_TO_METHOD.at(request_message.at(1));
    strcpy(message.code, request_message.substr(2, 3).c_str());
    output->write_fifo(static_cast<const void *>(&message), sizeof(message));

    struct portal_response_message_t response_message_fifo;
    ssize_t readedBytes = input->read_fifo(static_cast<void *>(&response_message_fifo), sizeof(response_message_fifo));
    logger->logMessage(INFO, "Received response from the portal: " + response_message_fifo.asString());

    return response_message_fifo.asString();
}