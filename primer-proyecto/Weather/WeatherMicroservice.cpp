#include "WeatherMicroservice.h"

WeatherMicroservice::WeatherMicroservice() {
  // Acá se debería abrir el archivo con las ciudades
  cities["Buenos Aires"] = new City("Buenos Aires",24,25,26);
}

void WeatherMicroservice::updateWeather(std::string name, float temperature, float pressure, float humidity) {
  cities[name]->setTemperature(temperature);
  cities[name]->setPressure(pressure);
  cities[name]->setHumidity(humidity);
}

std::list<float> WeatherMicroservice::getWeather(std::string name) {
  std::list<float> weather;
  weather.push_back(cities[name]->getTemperature());
  weather.push_back(cities[name]->getPressure());
  weather.push_back(cities[name]->getHumidity());
  return weather;
}
