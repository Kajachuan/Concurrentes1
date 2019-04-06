#ifndef WEATHERMICROSERVICE_H_
#define WEATHERMICROSERVICE_H_

#include <string>
#include <map>
#include <list>
#include "City.h"
#include "../Fifo/Fifo.h"
#include "../Fifo/FifoEscritura.h"
#include "../Fifo/FifoLectura.h"

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
    FifoLectura* input;
    FifoEscritura* output;
};

#endif /* WEATHERMICROSERVICE_H_ */
