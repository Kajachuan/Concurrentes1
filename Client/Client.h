#ifndef PRIMER_PROYECTO_CLIENT_H
#define PRIMER_PROYECTO_CLIENT_H

#include "../IPC/Fifo/Fifo.h"
#include "../IPC/Fifo/FifoReader.h"
#include "../IPC/Fifo/FifoWriter.h"

class Client {
  public:
    Client();
    ~Client();
    void write(std::string);
    std::string read();

  private:
    FifoReader* input;
    FifoWriter* output;
};


#endif //PRIMER_PROYECTO_CLIENT_H
