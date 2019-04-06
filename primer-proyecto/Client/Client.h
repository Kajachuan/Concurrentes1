#ifndef PRIMER_PROYECTO_CLIENT_H
#define PRIMER_PROYECTO_CLIENT_H

#include "../Fifo/Fifo.h"
#include "../Fifo/FifoEscritura.h"
#include "../Fifo/FifoLectura.h"

class Client {
  public:
    Client();
    ~Client();
    void write(std::string);
    std::string read();

  private:
    FifoLectura* input;
    FifoEscritura* output;
};


#endif //PRIMER_PROYECTO_CLIENT_H
