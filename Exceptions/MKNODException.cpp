#include <cstring>
#include "MKNODException.h"

using namespace std;

MKNODException::MKNODException(const string &description) {
    char *temp = new char[description.length() + 1];
    std::strcpy(temp, description.c_str());
    this->description = temp;
}

const char *MKNODException::what() const throw() {
    return description;
}