#ifndef PRIMER_PROYECTO_CRUDMICROSERVICECONTROLLER_H
#define PRIMER_PROYECTO_CRUDMICROSERVICECONTROLLER_H

#include <string>
#include <list>
#include <map>
#include <typeinfo>
#include "../Logger/LoggerClient.h"
#include "MicroserviceController.h"
#include "CRUDMicroserviceController.h"
#include "../Commons/Messages.h"
#include "../Commons/DataRecordManager.h"

template <class DataRecord>
class CRUDMicroserviceController: public MicroserviceController {
public:
    CRUDMicroserviceController(std::string registerRequestFifoPathName, std::string requestFifoPathName,
            INSTANCE_TYPE instanceType, DataRecordManager<DataRecord> *dataRecordManager);
    ~CRUDMicroserviceController();
    bool processRequest() override;
    void addRecord(std::string code, DataRecord record);
    std::list<DataRecord> getRecords();


private:
    static LoggerClient logger;
    std::map<std::string, DataRecord> data;
    DataRecordManager<DataRecord> *dataRecordManager;
};

template <class DataRecord>
LoggerClient CRUDMicroserviceController<DataRecord>::logger = LoggerClient("CRUDMicroserviceController" +
        std::string(typeid(DataRecord).name()));



template<class DataRecord>
CRUDMicroserviceController<DataRecord>::CRUDMicroserviceController(std::string registerRequestFifoPathName,
        std::string requestFifoPathName, INSTANCE_TYPE instanceType, DataRecordManager<DataRecord> *dataRecordManager):
        MicroserviceController(registerRequestFifoPathName, requestFifoPathName, instanceType) {
            this->dataRecordManager = dataRecordManager;
        }

template <class DataRecord>
CRUDMicroserviceController<DataRecord>::~CRUDMicroserviceController() = default;

template <class DataRecord>
void CRUDMicroserviceController<DataRecord>::addRecord(std::string code, DataRecord record) {
    data[code] = record;
}

template <class DataRecord>
bool CRUDMicroserviceController<DataRecord>::processRequest() {
    logger.logMessage(DEBUG, "Reading request fifo");
    MSRequest requestMessage{};
    ssize_t readedBytes = requestFifo->read_fifo(static_cast<void *>(&requestMessage), sizeof(MSRequest));
    if (readedBytes > 0) {
        logger.logMessage(DEBUG, "Received request: " + requestMessage.asString());
        if (responseFifos.count(requestMessage.responseFifoPath) == 0) {
            logger.logMessage(DEBUG, "Creating new response fifo: " + std::string(requestMessage.responseFifoPath));
            responseFifos[requestMessage.responseFifoPath] = new FifoWriter(requestMessage.responseFifoPath);
            responseFifos[requestMessage.responseFifoPath]->open_fifo();
        }
        if (requestMessage.closeConnection) {
            logger.logMessage(DEBUG, "Closing response fifo: " + std::string(requestMessage.responseFifoPath));
            delete responseFifos[requestMessage.responseFifoPath];
            responseFifos.erase(requestMessage.responseFifoPath);
        } else {
            PortalResponse response_message{};
            response_message.found = false;
            response_message.instanceType = dataRecordManager->getServiceName();
            if (requestMessage.method == READ) {
                logger.logMessage(DEBUG, std::string("Reading: ") + requestMessage.code);
                if (data.count(requestMessage.code) > 0) {
                    response_message.found = true;
                    dataRecordManager->setRecordToResponse(&response_message, data[requestMessage.code]);
                }
            } else {
                logger.logMessage(DEBUG, std::string("Writing: ") + requestMessage.code);
                addRecord(requestMessage.code, dataRecordManager->getRecordFromRequest(requestMessage));
                response_message.found = true;
                dataRecordManager->setRecordToResponse(&response_message, data[requestMessage.code]);
            }
            responseFifos[requestMessage.responseFifoPath]->write_fifo(static_cast<const void *>(&response_message),
                                                                       sizeof(PortalResponse));
            logger.logMessage(DEBUG, "Sending response: " + response_message.asString());
        }
        return requestMessage.closeConnection;
    }
    return false;
}

template <class DataRecord>
std::list<DataRecord> CRUDMicroserviceController<DataRecord>::getRecords() {
    std::list<DataRecord> records;
    for (typename std::map<std::string, DataRecord>::iterator it = data.begin(); it != data.end(); ++it)
        records.push_back(it->second);
    return records;
}


#endif //PRIMER_PROYECTO_CRUDMICROSERVICECONTROLLER_H
