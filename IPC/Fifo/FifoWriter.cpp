#include "FifoWriter.h"

FifoWriter::FifoWriter(const std::string path_name) : Fifo(path_name) {
}

FifoWriter::~FifoWriter() = default;

void FifoWriter::open_fifo() {
	fd = open(path_name.c_str(), O_WRONLY);
}

ssize_t FifoWriter::write_fifo(const void *buffer, size_t buffer_size) const {
	return write(fd, buffer, (buffer_size));
}
