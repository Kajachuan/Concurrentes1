#ifndef PRIMER_PROYECTO_CRUDMICROSERVICECONTROLLER_H
#define PRIMER_PROYECTO_CRUDMICROSERVICECONTROLLER_H

#include <string>
#include <list>
#include <map>
#include <typeinfo>
#include "../logger/Logger.h"
#include "MicroserviceController.h"
#include "CRUDMicroserviceController.h"
#include "../logger/Logger.h"
#include "../Commons/Messages.h"
#include "../Commons/DataRecordManager.h"

template <class DataRecord>
class CRUDMicroserviceController: public MicroserviceController {
public:
    CRUDMicroserviceController(std::string output_fifo_path_name, std::string input_fifo_path_name,
            DataRecordManager<DataRecord> *dataRecordManager);
    ~CRUDMicroserviceController();
    void processRequest() override;
    void addRecord(std::string code, DataRecord record);
    std::list<DataRecord> getRecords();


private:
    static Logger *logger;
    std::map<std::string, DataRecord> data;
    DataRecordManager<DataRecord> *dataRecordManager;
};

template <class DataRecord>
Logger *CRUDMicroserviceController<DataRecord>::logger = Logger::getInstance("CRUDMicroserviceController" +
                                                                             std::string(typeid(DataRecord).name()));

template <class DataRecord>
CRUDMicroserviceController<DataRecord>::CRUDMicroserviceController(std::string output_fifo_path_name,
                                                                   std::string input_fifo_path_name,
                                                                   DataRecordManager<DataRecord> *dataRecordManager)
        : MicroserviceController(output_fifo_path_name, input_fifo_path_name) {
    this->dataRecordManager = dataRecordManager;
}

template <class DataRecord>
CRUDMicroserviceController<DataRecord>::~CRUDMicroserviceController() = default;

template <class DataRecord>
void CRUDMicroserviceController<DataRecord>::addRecord(std::string code, DataRecord record) {
    data[code] = record;
}

template <class DataRecord>
void CRUDMicroserviceController<DataRecord>::processRequest() {
    logger->logMessage(DEBUG, "Reading request fifo");
    MSRequest requestMessage;
    ssize_t readedBytes = requestFifo->read_fifo(static_cast<void *>(&requestMessage), sizeof(MSRequest));
    if (readedBytes > 0) {
        logger->logMessage(DEBUG, "Received request: " + requestMessage.asString());
        if (responseFifos.count(requestMessage.responseFifoPath) == 0) {
            logger->logMessage(DEBUG, "Creating new response fifo: " + std::string(requestMessage.responseFifoPath));
            responseFifos[requestMessage.responseFifoPath] = new FifoWriter(requestMessage.responseFifoPath);
            responseFifos[requestMessage.responseFifoPath]->open_fifo();
        }
        if (requestMessage.closeConnection) {
            logger->logMessage(DEBUG, "Closing response fifo: " + std::string(requestMessage.responseFifoPath));
            delete responseFifos[requestMessage.responseFifoPath];
            responseFifos.erase(requestMessage.responseFifoPath);
        } else {
            PortalResponse response_message{};
            response_message.found = false;
            response_message.service = dataRecordManager->getServiceName();
            if (requestMessage.method == READ) {
                logger->logMessage(DEBUG, std::string("Reading: ") + requestMessage.code);
                if (data.count(requestMessage.code) > 0) {
                    response_message.found = true;
                    dataRecordManager->setRecordToResponse(&response_message, data[requestMessage.code]);
                }
            } else {
                logger->logMessage(DEBUG, std::string("Writing: ") + requestMessage.code);
                addRecord(requestMessage.code, dataRecordManager->getRecordFromRequest(requestMessage));
                response_message.found = true;
            }
            responseFifos[requestMessage.responseFifoPath]->write_fifo(static_cast<const void *>(&response_message),
                                                                       sizeof(PortalResponse));
            logger->logMessage(DEBUG, "Sending response: " + response_message.asString());
        }

    }
}

template <class DataRecord>
std::list<DataRecord> CRUDMicroserviceController<DataRecord>::getRecords() {
    std::list<DataRecord> records;
    for (typename std::map<std::string, DataRecord>::iterator it = data.begin(); it != data.end(); ++it)
        records.push_back(it->second);
    return records;
}


#endif //PRIMER_PROYECTO_CRUDMICROSERVICECONTROLLER_H