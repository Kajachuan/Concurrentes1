#include "ClientController.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
  std::cout << "Insert your command here: ";
  std::string command;
  std::cin >> command;
  std::cout << "creatin writer 22";
  auto * client = new ClientController("/tmp/client-portal", "/tmp/portal-client");
  std::cout << client->portal_request(command) << std::endl;
}
