#include "WeatherMicroserviceController.h"

#include "City.h"

WeatherMicroserviceController::WeatherMicroserviceController() {
  // Acá se debería abrir el archivo con las ciudades
  cities["Buenos Aires"] = new City("Buenos Aires",0,0,0);
}

void WeatherMicroserviceController::updateWeather(std::string name, float temperature, float pressure, float humidity) {
  cities[name]->setTemperature(temperature);
  cities[name]->setPressure(pressure);
  cities[name]->setHumidity(humidity);
}

std::list<float> WeatherMicroserviceController::getWeather(std::string name) {
  std::list<float> weather;
  weather.push_back(cities[name]->getTemperature());
  weather.push_back(cities[name]->getPressure());
  weather.push_back(cities[name]->getHumidity());
  return weather;
}

int main(int argc, char *argv[]) {
  WeatherMicroserviceController* weatherMicroservice = new WeatherMicroserviceController();
  // Acá debería comunicarse
  return 0;
}
