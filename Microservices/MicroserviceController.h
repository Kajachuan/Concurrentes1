#ifndef PRIMER_PROYECTO_MICROSERVICECONTROLLER_H
#define PRIMER_PROYECTO_MICROSERVICECONTROLLER_H

#include "../logger/Logger.h"
#include "../IPC/Fifo/FifoWriter.h"
#include "../IPC/Fifo/FifoReader.h"
#include <map>

class MicroserviceController {
public:

    MicroserviceController(std::string output_fifo_path_name, std::string input_fifo_path_name);

    ~MicroserviceController();

    virtual void processRequest() = 0;

protected:
    FifoReader *requestFifo;
    std::map<std::string, FifoWriter *> responseFifos;
private:
    static Logger *logger;
};


#endif //PRIMER_PROYECTO_MICROSERVICECONTROLLER_H
