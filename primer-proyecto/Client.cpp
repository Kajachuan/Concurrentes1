#include "Client.h"

Client::Client() : input(FifoLectura("")), output(FifoLectura("")) {
    input.abrir();
    output.abrir();
}

Client::~Client() {
    input.cerrar();
    output.cerrar();
}

void Client::write(std::string) {

}

std::string Client::read() {
    return std::string();
}

