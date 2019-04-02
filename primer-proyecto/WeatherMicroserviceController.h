#ifndef WEATHERMICROSERVICECONTROLLER_H
#define WEATHERMICROSERVICECONTROLLER_H

#include <string>
#include <map>
#include <list>
#include "City.h"

class WeatherMicroserviceController {
  public:
    WeatherMicroserviceController();
    void updateWeather(std::string, float, float, float);
    std::list<float> getWeather(std::string);

  private:
    std::map<std::string, City*> cities;
};

#endif
