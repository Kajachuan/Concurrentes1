#include <cstring>
#include <fstream>
#include "../logger/Logger.h"
#include "MicroserviceController.h"
#include "../Commons/Messages.h"
#include "CRUDMicroserviceController.h"

char LINES_SEPARATOR = ',';

int main(int argc, char const *argv[]) {
    auto logger = Logger::getInstance("ms main");
    Logger::setLogLevel(DEBUG);
    WeatherRecordManager recordManager;
    CRUDMicroserviceController<WeatherRecord> msController("/tmp/register-portal",
            "/tmp/query-ms", WEATHER_MICROSERVICE, &recordManager);

    std::ifstream records("../weather.csv");
    if(records.fail()) logger->logMessage(WARNING, "An error occurred while load DB");
    std::string line;
    while(getline(records, line)) {
      std::stringstream sline(line);
      std::string parsed;
      WeatherRecord wrec;
      int i = 0;
      while(getline(sline, parsed, LINES_SEPARATOR)) {
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

    logger->logMessage(DEBUG, "Start processing requests");
    while(!msController.processRequest()) {
        sleep(2);
    }

    logger->logMessage(DEBUG, "Saving DB");
    std::ofstream new_records("wheather.csv");
    std::list<WeatherRecord> data_to_save = msController.getRecords();
    for(auto record : data_to_save) {
        new_records << record.code << LINES_SEPARATOR << record.name << LINES_SEPARATOR << record
                .temperature << LINES_SEPARATOR << record.pressure << LINES_SEPARATOR << record
                .humidity;
        logger->logMessage(DEBUG, "Saved " + record.asString());
    }
    new_records.close();
    logger->logMessage(DEBUG, "Finished saving DB");

    Logger::endLogger();
}
