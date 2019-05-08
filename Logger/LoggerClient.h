#ifndef TP1_LOGGER_CLIENT_H
#define TP1_LOGGER_CLIENT_H

#include <iostream>
#include "../IPC/Fifo/FifoWriter.h"
#include "LoggerServer.h"

class LoggerClient {
private:
    static FifoWriter *logWriter;
    std::string reporter;

public:
    explicit LoggerClient(std::string reporter);
    void logMessage(LogLevel logLevel, const std::string& message);
    ~LoggerClient();
};


#endif //TP1_LOGGER_CLIENT_H
