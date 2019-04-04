#include "Client.h"
#include <iostream>

int main(int argc, char* argv[]) {
    auto client = Client();
    std::string command;
    std::cout << "Insert your command here: ";
    std::cin >> command;
    client.write(command);
    std::cout << client.read() << std::endl;
}