#ifndef TP1_LOGGER_H
#define TP1_LOGGER_H

#include <iostream>

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    static Logger *instance;
    static LogLevel runningLogLevel;

    static int fd;
    std::string reporter;

    Logger() = default;

public:
    static Logger *getInstance(std::string reporter);
    static void setLogLevel(LogLevel logLevel);
    static void endLogger();

    void logMessage(LogLevel logLevel, std::string message);
};


#endif //TP1_LOGGER_H
