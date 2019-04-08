#include "Client.h"
#include <iostream>

int main(int argc, char* argv[]) {
  std::cout << "Insert your command here: ";
  std::string command;
  std::cin >> command;
  Client* client = new Client();
  client->write(command);
  // std::cout << client->read() << std::endl;
}
