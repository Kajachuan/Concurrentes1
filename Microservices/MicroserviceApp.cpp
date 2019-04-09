#include <cstring>
#include <fstream>
#include "../logger/Logger.h"
#include "MicroserviceController.h"
#include "../Commons/Messages.h"
#include "CRUDMicroserviceController.h"

int main(int argc, char const *argv[]) {
    auto logger = Logger::getInstance("ms main");
    Logger::setLogLevel(DEBUG);
    WeatherRecordManager recordManager;
    CRUDMicroserviceController<WeatherRecord> msController("/tmp/ms-portal", "/tmp/portal-ms", &recordManager);

    std::ifstream records("weather.csv");
    if(records.fail()) logger->logMessage(WARNING, "An error occurred while load DB");
    std::string line;
    while(getline(records, line)) {
      std::stringstream sline(line);
      std::string parsed;
      WeatherRecord wrec;
      int i = 0;
      while(getline(sline, parsed, ',')) {
        if(i == 0) strcpy(wrec.code, parsed.c_str());
        else if(i == 1) strcpy(wrec.name, parsed.c_str());
        else if(i == 2) wrec.temperature = stof(parsed);
        else if(i == 3) wrec.pressure = stof(parsed);
        else if(i == 4) wrec.humidity = stof(parsed);
        i++;
      }
      msController.addRecord(wrec.code, wrec);
      logger->logMessage(DEBUG, "Load initial DB" + wrec.asString());
    }
    records.close();

    while(true) {
        msController.processRequest();
        logger->logMessage(DEBUG, "Processed request");
        sleep(2);
    }
    Logger::endLogger();
}
