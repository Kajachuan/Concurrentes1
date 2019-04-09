#ifndef PRIMER_PROYECTO_MICROSERVICECONTROLLER_H
#define PRIMER_PROYECTO_MICROSERVICECONTROLLER_H

const int BUFFER_SIZE = 100;

#include "../logger/Logger.h"
#include "../IPC/Fifo/FifoWriter.h"
#include "../IPC/Fifo/FifoReader.h"
#include <map>

class MicroserviceController {
public:

    MicroserviceController(std::string output_fifo_path_name, std::string input_fifo_path_name);

    ~MicroserviceController();

    void processRequest();

private:
    static Logger *logger;
    FifoReader *requestFifo;
    std::map<std::string, FifoWriter *> responseFifos;
};


#endif //PRIMER_PROYECTO_MICROSERVICECONTROLLER_H
