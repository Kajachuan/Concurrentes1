#ifndef WEATHERMICROSERVICE_H_
#define WEATHERMICROSERVICE_H_

#include <string>
#include <map>
#include <list>
#include "../../IPC/Fifo/Fifo.h"
#include "../../IPC/Fifo/FifoReader.h"
#include "../../IPC/Fifo/FifoWriter.h"
#include "../MicroserviceController.h"

struct City {
    std::string name;
    float temperature;
    float pressure;
    float humidity;
};

class WeatherMicroserviceController: public MicroserviceController {
  public:
    WeatherMicroserviceController(std::string output_fifo_path_name, std::string input_fifo_path_name);
    ~WeatherMicroserviceController();
    void processRequest() override;

  private:
    static Logger *logger;
    std::map<std::string, City> cities;
    void updateWeather(std::string, std::string, float, float, float);
};

#endif /* WEATHERMICROSERVICE_H_ */
