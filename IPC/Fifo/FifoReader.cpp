#include <cstring>
#include "FifoReader.h"
#include "../../Exceptions/FileExistsException.h"

FifoReader::FifoReader(const std::string nombre) : Fifo(nombre) {
}

FifoReader::~FifoReader() = default;

void FifoReader::open_fifo() {
    fd = open(path_name.c_str(), O_RDONLY);
    if (fd < 0) {
        throw FileExistsException("Error during fifo creation, file: " + path_name +
                                  ". Error: "+ strerror(errno));
    }
}

ssize_t FifoReader::read_fifo(void *buffer, size_t buffer_size) const {
    return read(fd, buffer, buffer_size);
}
