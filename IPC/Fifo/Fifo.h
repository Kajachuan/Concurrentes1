#ifndef FIFO_H_
#define FIFO_H_

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class Fifo {
public:
    explicit Fifo(std::string path_name);

    virtual ~Fifo();

    virtual void open_fifo() = 0;

    void deleteFifo();

protected:
    std::string path_name;
    int fd;
};

#endif /* FIFO_H_ */
