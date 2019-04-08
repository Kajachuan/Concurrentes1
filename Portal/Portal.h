#ifndef PORTAL_H_
#define PORTAL_H_

#include <string>
#include "../primer-proyecto/Fifo/Fifo.h"
#include "../primer-proyecto/Fifo/FifoEscritura.h"
#include "../primer-proyecto/Fifo/FifoLectura.h"

class Portal {
  public:
    Portal();
    ~Portal();
    void writeToClient(std::string);
    void writeToWeather(std::string);
    std::string readFromClient();
    std::string readFromWeather();

  private:
    FifoLectura* inputClient;
    FifoLectura* inputWeather;
    FifoEscritura* outputClient;
    FifoEscritura* outputWeather;
};

#endif /* PORTAL_H_ */
