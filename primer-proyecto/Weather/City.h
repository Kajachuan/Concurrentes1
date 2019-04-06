#ifndef CITY_H_
#define CITY_H_

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

#endif /* CITY_H_ */
