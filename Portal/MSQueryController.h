#ifndef MS_QUERY_CONTROLLER_H_
#define MS_QUERY_CONTROLLER_H_

#include <string>

#include "../IPC/Fifo/FifoReader.h"
#include "../IPC/Fifo/FifoWriter.h"
#include "../Commons/Messages.h"
#include "../logger/Logger.h"

class MSQueryController {
public:
    MSQueryController(std::string clientRequestFifoPath, std::string clientResponseFifoPath,
                      std::string servicesResponseFifoPath, FifoWriter *servicesRequestFifos);

    ~MSQueryController();

    bool process_requests();

private:
    PortalResponse getMSResponse(MSRequest);
    static Logger *logger;
    FifoReader *clientRequestFifo;
    FifoWriter *clientResponseFifo;
    FifoReader *servicesResponseFifo;
    FifoWriter *servicesRequestFifos;
    std::string servicesResponseFifoPath;
};

#endif /* MS_QUERY_CONTROLLER_H_ */
