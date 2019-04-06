#include "WeatherMicroservice.h"
#include <iostream>

int main(int argc, char const *argv[]) {
  WeatherMicroservice* weatherMicroservice = new WeatherMicroservice();
  std::cout << "I read: " << weatherMicroservice->read() << std::endl;
  // std::list<float> weather = weatherMicroservice->getWeather("Buenos Aires");
  // std::string temperature = std::to_string(weather.pop_front());
  // std::string pressure = std::to_string(weather.pop_front());
  // std::string humidity = std::to_string(weather.pop_front());
  // weatherMicroservice->write("La temperatura");
  return 0;
}
