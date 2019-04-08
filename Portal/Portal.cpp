#include "Portal.h"

#define BUFFSIZE 100

Portal::Portal() {
  inputClient = new FifoReader("../client-portal");
  inputWeather = new FifoReader("../weather-portal");
  // outputClient = new FifoEscritura("../portal-client");
  outputWeather = new FifoWriter("../portal-weather");
  inputClient->open_fifo();
  inputWeather->open_fifo();
  // outputClient->abrir();
  outputWeather->open_fifo();
}

Portal::~Portal() = default;

void Portal::writeToWeather(std::string message) {
  outputWeather->write_fifo(static_cast<const void*>(message.c_str()), message.length());
}

std::string Portal::readFromClient() {
  char buffer[BUFFSIZE];
  ssize_t bytes = inputClient->read_fifo(static_cast<void*>(buffer), BUFFSIZE);
  std::string message = buffer;
  message.resize(bytes);
  return message;
}

std::string Portal::readFromWeather() {
  char buffer[BUFFSIZE];
  ssize_t bytes = inputWeather->read_fifo(static_cast<void*>(buffer), BUFFSIZE);
  std::string message = buffer;
  message.resize(bytes);
  return message;
}
