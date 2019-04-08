#ifndef WEATHERMICROSERVICE_H_
#define WEATHERMICROSERVICE_H_

#include <string>
#include <map>
#include <list>
#include "City.h"
#include "../IPC/Fifo/Fifo.h"
#include "../IPC/Fifo/FifoReader.h"
#include "../IPC/Fifo/FifoWriter.h"

class WeatherMicroservice {
  public:
    WeatherMicroservice();
    ~WeatherMicroservice();
    void updateWeather(std::string, float, float, float);
    std::list<float> getWeather(std::string);
    void write(std::string);
    std::string read();

  private:
    std::map<std::string, City*> cities;
    FifoReader* input;
    FifoWriter* output;
};

#endif /* WEATHERMICROSERVICE_H_ */
