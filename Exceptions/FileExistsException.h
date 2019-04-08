#ifndef FILEEXISTSEXCEPTION_H
#define FILEEXISTSEXCEPTION_H

#include <exception>
#include <cstring>
#include <string>

using namespace std;

class FileExistsException : public exception {
public:
    explicit FileExistsException(const string &description);
    const char *what() const throw() override;

private:
    const char *description;
};

#endif //FILEEXISTSEXCEPTION_H
