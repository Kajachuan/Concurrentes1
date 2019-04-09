

#include "../logger/Logger.h"
#include "MicroserviceController.h"
#include "Weather/WeatherMicroserviceController.h"

int main(int argc, char const *argv[]) {
    auto logger = Logger::getInstance("ms main");
    Logger::setLogLevel(DEBUG);
    auto msController = WeatherMicroserviceController("/tmp/ms-portal", "/tmp/portal-ms");
    while(true) {
        msController.processRequest();
        logger->logMessage(DEBUG, "Processed request");
        sleep(2);
    }
    Logger::endLogger();
}