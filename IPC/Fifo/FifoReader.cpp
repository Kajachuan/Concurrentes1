#include "FifoReader.h"

FifoReader::FifoReader(const std::string nombre) : Fifo(nombre) {
}

FifoReader::~FifoReader() = default;

void FifoReader::open_fifo() {
    fd = open(path_name.c_str(), O_RDONLY);
}

ssize_t FifoReader::read_fifo(void *buffer, size_t buffer_size) const {
    return read(fd, buffer, buffer_size);
}
