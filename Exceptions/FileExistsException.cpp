#include <cstring>
#include "FileExistsException.h"

using namespace std;

FileExistsException::FileExistsException(const string &description) {
    char *temp = new char[description.length() + 1];
    std::strcpy(temp, description.c_str());
    this->description = temp;
}

const char *FileExistsException::what() const throw() {
    return description;
}