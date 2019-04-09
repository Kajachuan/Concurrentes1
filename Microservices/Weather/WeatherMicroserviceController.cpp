#include "WeatherMicroserviceController.h"
#include "../../Commons/Messages.h"

Logger *WeatherMicroserviceController::logger = Logger::getInstance("WeatherMicroserviceController");

WeatherMicroserviceController::WeatherMicroserviceController(std::string output_fifo_path_name, std::string input_fifo_path_name)
        : MicroserviceController(output_fifo_path_name, input_fifo_path_name) {
  // Acá se debería abrir el archivo con las ciudades
  cities["bas"] = City{"Buenos Aires", 24, 25, 26};
}

WeatherMicroserviceController::~WeatherMicroserviceController() = default;

void WeatherMicroserviceController::updateWeather(std::string code, std::string name, float temperature, float pressure,
        float humidity) {
  if (cities.count(name) == 0) {
    cities[code] = City{name, temperature, pressure, humidity};
  } else {
    cities[code].temperature = temperature;
    cities[code].name = name;
    cities[code].pressure = pressure;
    cities[code].humidity = humidity;
  }
}

void WeatherMicroserviceController::processRequest() {
  logger->logMessage(DEBUG, "Reading request fifo");
  char buffer[BUFFER_SIZE];
  WeatherRequest requestMessage;
  ssize_t readedBytes = requestFifo->read_fifo(static_cast<void *>(&requestMessage), sizeof(WeatherRequest));
  if (readedBytes > 0) {
    logger->logMessage(DEBUG, "Received request: " + requestMessage.asString());
    if (responseFifos.count(requestMessage.responseFifoPath) == 0) {
      logger->logMessage(DEBUG, "Creating new response fifo: " + std::string(requestMessage.responseFifoPath));
      responseFifos[requestMessage.responseFifoPath] = new FifoWriter(requestMessage.responseFifoPath);
      responseFifos[requestMessage.responseFifoPath]->open_fifo();
    }
    if (requestMessage.closeConnection) {
      logger->logMessage(DEBUG, "Closing response fifo: " + std::string(requestMessage.responseFifoPath));
      delete responseFifos[requestMessage.responseFifoPath];
      responseFifos.erase(requestMessage.responseFifoPath);
    } else {
      portal_response_message_t response_message{WEATHER, 0, 0, 0, 0, false};
      if (requestMessage.method == READ) {
        logger->logMessage(DEBUG, std::string("Reading: ") + requestMessage.code);
        if (cities.count(requestMessage.code) > 0) {
          response_message.found = true;
          response_message.temperature = cities[requestMessage.code].temperature;
          response_message.humidity = cities[requestMessage.code].humidity;
          response_message.pressure = cities[requestMessage.code].pressure;
        }
      } else {
        logger->logMessage(DEBUG, std::string("Writing: ") + requestMessage.code);
        updateWeather(requestMessage.code, requestMessage.name, requestMessage.temperature, requestMessage.pressure,
                requestMessage.humidity);
        response_message.found = true;
      }
      responseFifos[requestMessage.responseFifoPath]->write_fifo(static_cast<const void *>(&response_message),
                                                                 sizeof(portal_response_message_t));
      logger->logMessage(DEBUG, "Sending response: " + response_message.asString());
    }

  }
}