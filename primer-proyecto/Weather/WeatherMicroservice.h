#ifndef WEATHERMICROSERVICE_H_
#define WEATHERMICROSERVICE_H_

#include <string>
#include <map>
#include <list>
#include "City.h"

class WeatherMicroservice {
  public:
    WeatherMicroservice();
    void updateWeather(std::string, float, float, float);
    std::list<float> getWeather(std::string);

  private:
    std::map<std::string, City*> cities;
};

#endif /* WEATHERMICROSERVICE_H_ */
