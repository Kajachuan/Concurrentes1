#ifndef FIFO_READER_H_
#define FIFO_READER_H_

#include "Fifo.h"

class FifoReader : public Fifo {
public:
    explicit FifoReader(std::string nombre);

    ~FifoReader() override;

    void open_fifo() override;

    ssize_t read_fifo(void *buffer, size_t buffer_size) const;
};

#endif /* FIFO_READER_H_ */
