#include "WeatherMicroservice.h"

#define BUFFSIZE 100

WeatherMicroservice::WeatherMicroservice() {
  // Acá se debería abrir el archivo con las ciudades
  cities["Buenos Aires"] = new City("Buenos Aires",24,25,26);
  input = new FifoLectura("../portal-weather");
  output = new FifoEscritura("../weather-portal");
  input->abrir();
  output->abrir();
}

WeatherMicroservice::~WeatherMicroservice() {
  input->cerrar();
  output->cerrar();
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

void WeatherMicroservice::write(std::string message) {
  output->escribir(static_cast<const void*>(message.c_str()), message.length());
}

std::string WeatherMicroservice::read() {
  char buffer[BUFFSIZE];
  ssize_t bytes = input->leer(static_cast<void*>(buffer), BUFFSIZE);
  std::string message = buffer;
  message.resize(bytes);
  return message;
}
