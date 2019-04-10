#include "Fifo.h"
#include "../../Exceptions/MKNODException.h"
#include "../../Exceptions/FileExistsException.h"
#include <iostream>

using namespace std;

Fifo::Fifo(const std::string path_name) : path_name(path_name), fd(-1) {
    // Checks if the file already exists before creating the fifo
    if (access(path_name.c_str(), F_OK) < 0) {
        if (mknod(path_name.c_str(), S_IFIFO | 0666, 0) < 0) {
            throw MKNODException("Error during fifo creation with name: " + path_name + ". Error: " + strerror(errno));
        }
    } else {
        struct stat file_stat{};
        // Checks if the existing file is a fifo
        if (not fstat(open(path_name.c_str(), O_RDONLY | O_NONBLOCK), &file_stat) && not S_ISFIFO(file_stat.st_mode)) {
            throw FileExistsException("Error during fifo creation, file: " + path_name +
                                      " already exists and it is not a fifo");
        }
    }
}

Fifo::~Fifo() {
    close(fd);
}

void Fifo::deleteFifo() {
    unlink(path_name.c_str());
}
