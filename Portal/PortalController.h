#ifndef PORTAL_CONTROLLER_H_
#define PORTAL_CONTROLLER_H_

#include <string>

#include "../IPC/Fifo/FifoReader.h"
#include "../IPC/Fifo/FifoWriter.h"

class PortalController {
public:
    PortalController(std::string output_fifo_path_name, std::string input_fifo_path_name);

    ~PortalController();

    void process_requests();

private:
    FifoReader *input;
    FifoWriter *output;
};

#endif /* PORTAL_CONTROLLER_H_ */
