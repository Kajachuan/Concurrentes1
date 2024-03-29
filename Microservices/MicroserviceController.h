#ifndef PRIMER_PROYECTO_MICROSERVICECONTROLLER_H
#define PRIMER_PROYECTO_MICROSERVICECONTROLLER_H

#include "../Logger/LoggerClient.h"
#include "../IPC/Fifo/FifoWriter.h"
#include "../IPC/Fifo/FifoReader.h"
#include "../Commons/Messages.h"
#include <map>

class MicroserviceController {
public:
    MicroserviceController(std::string registerRequestFifoPathName, std::string requestFifoPathName,
                           INSTANCE_TYPE instanceType);

    ~MicroserviceController();

    virtual bool processRequest() = 0;

protected:
    FifoReader *requestFifo;
    std::map<std::string, FifoWriter *> responseFifos;
private:
    static LoggerClient logger;
};


#endif //PRIMER_PROYECTO_MICROSERVICECONTROLLER_H
