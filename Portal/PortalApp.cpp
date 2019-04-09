#include <iostream>
#include "PortalController.h"

int main(int argc, char const *argv[]) {

  std::cout << "Insert your command here33: " <<std::endl ;
  auto portal = PortalController("/tmp/portal-client", "/tmp/client-portal");
  while(true) {
    sleep(2);
    portal.process_requests();
  }
}
