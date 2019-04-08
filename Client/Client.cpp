#include "Client.h"

Client::Client() {
  // input = new FifoLectura("../portal-client");
  output = new FifoWriter("../client-portal");
  // input->abrir();
}

Client::~Client() = default;

void Client::write(std::string message) {
  output->write_fifo(static_cast<const void*>(message.c_str()), message.length());
}

std::string Client::read() {
  return std::string();
}
