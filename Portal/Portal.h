#ifndef PORTAL_H_
#define PORTAL_H_

#include <string>
#include "../IPC/Fifo/Fifo.h"
#include "../IPC/Fifo/FifoReader.h"
#include "../IPC/Fifo/FifoWriter.h"

class Portal {
  public:
    Portal();
    ~Portal();
    void writeToClient(std::string);
    void writeToWeather(std::string);
    std::string readFromClient();
    std::string readFromWeather();

  private:
    FifoReader* inputClient;
    FifoReader* inputWeather;
    FifoWriter* outputClient;
    FifoWriter* outputWeather;
};

#endif /* PORTAL_H_ */
