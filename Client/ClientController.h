#ifndef PRIMER_PROYECTO_CLIENT_H
#define PRIMER_PROYECTO_CLIENT_H

#include "../IPC/Fifo/FifoReader.h"
#include "../IPC/Fifo/FifoWriter.h"
#include "../Commons/Messages.h"
#include "../logger/Logger.h"

class ClientController {
public:
    static Logger *logger;
    ClientController(std::string registerRequestFifoPathName, std::string responseFifoPathName);

    ~ClientController();

    std::string portal_request(std::string request_message);

private:
    FifoReader *responseFifo;
    FifoWriter *requestFifo;
};


#endif //PRIMER_PROYECTO_CLIENT_H
