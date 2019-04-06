#include "Client.h"

Client::Client() {
  // input = new FifoLectura("../portal-client");
  output = new FifoEscritura("../client-portal");
  // input->abrir();
  output->abrir();
}

Client::~Client() {
  // input->cerrar();
  output->cerrar();
}

void Client::write(std::string message) {
  output->escribir(static_cast<const void*>(message.c_str()), message.length());
}

std::string Client::read() {
  return std::string();
}
