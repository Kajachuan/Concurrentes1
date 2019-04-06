#include "Portal.h"

#define BUFFSIZE 100

Portal::Portal() {
  inputClient = new FifoLectura("../client-portal");
  // inputWeather = new FifoLectura("../weather-portal");
  // outputClient = new FifoEscritura("../portal-client");
  outputWeather = new FifoEscritura("../portal-weather");
  inputClient->abrir();
  // inputWeather->abrir();
  // outputClient->abrir();
  outputWeather->abrir();
}

Portal::~Portal() {
  inputClient->cerrar();
  // inputWeather->cerrar();
  // outputClient->cerrar();
  outputWeather->cerrar();
}

void Portal::writeToWeather(std::string message) {
  outputWeather->escribir(static_cast<const void*>(message.c_str()), message.length());
}

std::string Portal::readFromClient() {
  char buffer[BUFFSIZE];
  ssize_t bytes = inputClient->leer(static_cast<void*>(buffer), BUFFSIZE);
  std::string message = buffer;
  message.resize(bytes);
  return message;
}

std::string Portal::readFromWeather() {
  char buffer[BUFFSIZE];
  ssize_t bytes = inputWeather->leer(static_cast<void*>(buffer), BUFFSIZE);
  std::string message = buffer;
  message.resize(bytes);
  return message;
}
