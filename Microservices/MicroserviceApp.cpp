#include <cstring>
#include "../logger/Logger.h"
#include "MicroserviceController.h"
#include "../Commons/Messages.h"
#include "CRUDMicroserviceController.h"

int main(int argc, char const *argv[]) {
    auto logger = Logger::getInstance("ms main");
    Logger::setLogLevel(DEBUG);
    WeatherRecordManager recordManager;
    CRUDMicroserviceController<WeatherRecord> msController("/tmp/ms-portal", "/tmp/portal-ms", &recordManager);
    WeatherRecord wrec;
    wrec.pressure = 23;
    wrec.temperature = 21;
    wrec.humidity = 243;
    strcpy(wrec.name, "Buenos aires");
    logger->logMessage(DEBUG, "Load initial DB" + wrec.asString());
    msController.addRecord("bas", wrec);
    while(true) {
        msController.processRequest();
        logger->logMessage(DEBUG, "Processed request");
        sleep(2);
    }
    Logger::endLogger();
}

