#ifndef PRIMER_PROYECTO_CLIENT_H
#define PRIMER_PROYECTO_CLIENT_H

#include "../primer-proyecto/Fifo/Fifo.h"
#include "../primer-proyecto/Fifo/FifoEscritura.h"
#include "../primer-proyecto/Fifo/FifoLectura.h"

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
