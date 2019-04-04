#ifndef PRIMER_PROYECTO_CLIENT_H
#define PRIMER_PROYECTO_CLIENT_H

#include "Fifo.h"
#include "FifoEscritura.h"
#include "FifoLectura.h"

class Client {
public:
    Client();
    ~Client();
    void write(std::string);
    std::string read();

private:
    Fifo input;
    Fifo output;
};


#endif //PRIMER_PROYECTO_CLIENT_H
