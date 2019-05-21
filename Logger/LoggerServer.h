#ifndef TP1_LOGGER_SERVER_H
#define TP1_LOGGER_SERVER_H

#include <iostream>
#include <cstring>
#include "../IPC/Fifo/FifoReader.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

static const char *logLevelNames[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
static const char LOG_FIFO_PATH[] = "/tmp/logs";

struct LogMessage {
    char timeStamp[30];
    char logText[200];
    char reporter[50];
    LogLevel logLevel;

    void toString(char *buffer) {
        strcpy(buffer, timeStamp);
        strcpy(&buffer[30], logText);
        strcpy(&buffer[230], reporter);
        char logLevelBuffer[2];
        snprintf(logLevelBuffer, sizeof(logLevelBuffer), "%d", logLevel);
        strcpy(&buffer[280], logLevelBuffer);
    }

    static LogMessage FromString(const char *buffer) {
        LogMessage logMessage{};
        for (int i = 0; i < 30; i++)
            logMessage.timeStamp[i] = buffer[i];
        for (int i = 0; i < 200; i++)
            logMessage.logText[i] = buffer[30 + i];
        for (int i = 0; i < 50; i++)
            logMessage.reporter[i] = buffer[230 + i];
        logMessage.logLevel = static_cast<LogLevel>(atoi(&buffer[280]));
        return logMessage;
    }
};

class LoggerServer {
private:
    static LoggerServer *instance;
    static LogLevel runningLogLevel;
    static FifoReader *logReader;
    static int loggerFileDescriptor;

    LoggerServer() = default;

public:
    static LoggerServer *getInstance();

    static void setLogLevel(LogLevel logLevel);

    void readNewLogs();

    static void endLogger();
};


#endif //TP1_LOGGER_SERVER_H
