#ifndef MKNOD_EXCEPTION_H
#define MKNOD_EXCEPTION_H

#include <exception>
#include <cstring>
#include <string>

using namespace std;

class MKNODException : public exception {
public:
    explicit MKNODException(const string &description);
    const char *what() const throw() override;

private:
    const char *description;
};

#endif //MKNOD_EXCEPTION_H
