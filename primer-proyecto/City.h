#ifndef CITY_H
#define CITY_H

#include <string>

class City {
  public:
    City(std::string, float, float, float);
    void setTemperature(float);
    void setPressure(float);
    void setHumidity(float);
    float getTemperature();
    float getPressure();
    float getHumidity();

  private:
    std::string name;
    float temperature;
    float pressure;
    float humidity;
};

#endif
