#ifndef FIFO_WRITER_H_
#define FIFO_WRITER_H_

#include "Fifo.h"

class FifoWriter : public Fifo {
public:
    explicit FifoWriter(std::string path_name);

    ~FifoWriter() override;

    void open_fifo() override;

    ssize_t write_fifo(const void *buffer, size_t buffer_size) const;
};

#endif /* FIFO_WRITER_H_ */
