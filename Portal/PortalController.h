#ifndef PORTAL_CONTROLLER_H_
#define PORTAL_CONTROLLER_H_

#include <string>

#include "../IPC/Fifo/FifoReader.h"
#include "../IPC/Fifo/FifoWriter.h"
#include "../Commons/Messages.h"
#include "../logger/Logger.h"

class PortalController {
public:
    PortalController(std::string responseFifoPath, std::string requestFifoPath,
            std::string registerMSFifoPath);

    ~PortalController();

    void process_requests();

private:
    static Logger *logger;
    FifoReader *requestFifo;
    FifoWriter *requestMSFifo;
};

#endif /* PORTAL_CONTROLLER_H_ */
