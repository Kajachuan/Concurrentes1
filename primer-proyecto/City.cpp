#include "City.h"

City::City(std::string name, float temperature, float pressure, float humidity) {
  this->name = name;
  this->temperature = temperature;
  this->pressure = pressure;
  this->humidity = humidity;
}

void City::setTemperature(float temperature) {
  this->temperature = temperature;
}

void City::setPressure(float pressure) {
  this->pressure = pressure;
}

void City::setHumidity(float humidity) {
  this->humidity = humidity;
}

float City::getTemperature() {
  return temperature;
}

float City::getPressure() {
  return pressure;
}

float City::getHumidity() {
  return humidity;
}
