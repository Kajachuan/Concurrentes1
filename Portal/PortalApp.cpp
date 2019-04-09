#include <iostream>
#include "PortalController.h"

int main(int argc, char const *argv[]) {
  auto logger = Logger::getInstance("portal main");
  Logger::setLogLevel(DEBUG);
  auto portal = PortalController("/tmp/portal-client", "/tmp/client-portal", "/tmp/ms-portal");
  while(true) {
    logger->logMessage(DEBUG, "processed message");
    sleep(2);
    portal.process_requests();
  }
  Logger::endLogger();
}
