#ifndef PORTAL_CONTROLLER_H_
#define PORTAL_CONTROLLER_H_

#include <string>
#include <vector>

#include "../IPC/Fifo/FifoReader.h"
#include "../IPC/Fifo/FifoWriter.h"
#include "../Commons/Messages.h"
#include "../logger/Logger.h"

class PortalController {
public:
    explicit PortalController(std::string connectionRequestFifoPath);

    ~PortalController();

    int processConnectionRequests();

    void endPortal();

private:
    static Logger *logger;
    FifoReader *connectionRequestFifo;
    FifoWriter *servicesRequestFifos;
    int forkedChilds;
};

#endif /* PORTAL_CONTROLLER_H_ */
