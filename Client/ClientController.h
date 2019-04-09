#ifndef PRIMER_PROYECTO_CLIENT_H
#define PRIMER_PROYECTO_CLIENT_H

#include "../IPC/Fifo/FifoReader.h"
#include "../IPC/Fifo/FifoWriter.h"
#include "../Commons/Message.h"
#include "../logger/Logger.h"

class ClientController {
public:
    static Logger *logger;
    ClientController(std::string output_fifo_path_name, std::string input_fifo_path_name);

    ~ClientController();

    std::string portal_request(std::string request_message);

private:
    FifoReader *input;
    FifoWriter *output;
};


#endif //PRIMER_PROYECTO_CLIENT_H
