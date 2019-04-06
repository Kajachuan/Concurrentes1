#include "Portal.h"
#include <iostream>

int main(int argc, char const *argv[]) {
  std::cout << "Welcome" << std::endl;
  Portal* portal = new Portal();
  std::cout << "I read: " << portal->readFromClient() << std::endl;
  portal->writeToWeather("temperature");
  // std::cout << "Temperature: " << portal->readFromWeather() << std::endl;
  return 0;
}
