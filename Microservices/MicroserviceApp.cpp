#include <cstring>
#include <fstream>
#include "../Logger/LoggerClient.h"
#include "MicroserviceController.h"
#include "../Commons/DataRecordManager.h"
#include "CRUDMicroserviceController.h"

char LINES_SEPARATOR = ',';


int main(int argc, char const *argv[]) {
    auto logger = LoggerClient("ms main");
    pid_t pid = fork();
    if (pid == 0) {
        CRUDMicroserviceController<WeatherRecord> *msController;
        DataRecordManager<WeatherRecord> *recordManager = new WeatherRecordManager();
        msController = new CRUDMicroserviceController<WeatherRecord>("/tmp/register-portal",
                                                                     "/tmp/query-msw", WEATHER_MICROSERVICE,
                                                                     recordManager);


        std::ifstream records("../weather.csv");
        if (records.fail()) logger.logMessage(WARNING, "An error occurred while load DB");
        std::string line;
        while (getline(records, line)) {
            std::stringstream sline(line);
            std::string parsed;
            WeatherRecord wrec;
            int i = 0;
            while (getline(sline, parsed, LINES_SEPARATOR)) {
                if (i == 0) strcpy(wrec.code, parsed.c_str());
                else if (i == 1) strcpy(wrec.name, parsed.c_str());
                else if (i == 2) wrec.temperature = stof(parsed);
                else if (i == 3) wrec.pressure = stof(parsed);
                else if (i == 4) wrec.humidity = stof(parsed);
                i++;
            }
            msController->addRecord(wrec.code, wrec);
            logger.logMessage(DEBUG, "Load initial DB" + wrec.asString());
        }
        records.close();

        logger.logMessage(DEBUG, "Start processing requests");
        while (!msController->processRequest()) { }

        logger.logMessage(DEBUG, "Saving DB");
        std::ofstream new_records("../weather.csv");
        std::list<WeatherRecord> data_to_save = msController->getRecords();
        for (auto record : data_to_save) {
            new_records << record.code << LINES_SEPARATOR << record.name << LINES_SEPARATOR << record
                    .temperature << LINES_SEPARATOR << record.pressure << LINES_SEPARATOR << record
                                .humidity << std::endl;
            logger.logMessage(DEBUG, "Saved " + record.asString());
        }
        new_records.close();
        logger.logMessage(DEBUG, "Finished saving DB");
        delete msController;
    } else {
        CRUDMicroserviceController<ExchangeRecord> *msController;
        DataRecordManager<ExchangeRecord> *recordManager = new ExchangeRecordManager();
        msController = new CRUDMicroserviceController<ExchangeRecord>("/tmp/register-portal",
                                                                      "/tmp/query-mse", EXCHANGE_MICROSERVICE,
                                                                      recordManager);
        std::ifstream records("../exchange.csv");
        if (records.fail()) logger.logMessage(WARNING, "An error occurred while load DB");
        std::string line;
        while (getline(records, line)) {
            std::stringstream sline(line);
            std::string parsed;
            ExchangeRecord wrec{};
            int i = 0;
            while (getline(sline, parsed, LINES_SEPARATOR)) {
                if (i == 0) strcpy(wrec.code, parsed.c_str());
                else if (i == 1) strcpy(wrec.name, parsed.c_str());
                else if (i == 2) wrec.exchange = stof(parsed);
                i++;
            }
            msController->addRecord(wrec.code, wrec);
            logger.logMessage(DEBUG, "Load initial DB" + wrec.asString());
        }
        records.close();

        logger.logMessage(DEBUG, "Start processing requests");
        while (!msController->processRequest()) {}

        logger.logMessage(DEBUG, "Saving DB");
        std::ofstream new_records("../exchange.csv");
        std::list<ExchangeRecord> data_to_save = msController->getRecords();
        for (auto record : data_to_save) {
            new_records << record.code << LINES_SEPARATOR << record.name << LINES_SEPARATOR << record.exchange << std::endl;
            logger.logMessage(DEBUG, "Saved " + record.asString());
        }
        new_records.close();
        logger.logMessage(DEBUG, "Finished saving DB");
        delete msController;
    }
}
