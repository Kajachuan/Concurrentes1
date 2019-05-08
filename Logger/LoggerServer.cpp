#include <utility>

#include "LoggerServer.h"
#include "../IPC/Fifo/FifoReader.h"
#include "../IPC/Fifo/FifoWriter.h"
#include <ctime>
#include <fcntl.h>
#include <unistd.h>

FifoReader *LoggerServer::logReader = nullptr;
static const char LOG_FILE[] = "logs/log.txt";
LoggerServer *LoggerServer::instance = nullptr;
LogLevel LoggerServer::runningLogLevel = INFO;
int LoggerServer::loggerFileDescriptor = 0;

LoggerServer *LoggerServer::getInstance() {
    if (instance == nullptr) {
        instance = new LoggerServer();
        loggerFileDescriptor = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0666);
        logReader = new FifoReader(LOG_FIFO_PATH);
        logReader->open_fifo();
    }
    return instance;
}

void LoggerServer::setLogLevel(LogLevel logLevel) {
    runningLogLevel = logLevel;
}

void LoggerServer::readNewLogs() {
    int buffer_size = 281;
    char buffer[buffer_size];
    if (logReader->read_fifo(buffer, buffer_size) > 0) {
        LogMessage logMessage = LogMessage::FromString(buffer);
        if (runningLogLevel <= logMessage.logLevel) {
            std::string reporterMessage = std::string(logMessage.timeStamp) + " :: " +
                    logLevelNames[logMessage.logLevel] + " :: " + logMessage.reporter + " :: " + logMessage.logText;
            std::string reporterMessageWithEndLine = reporterMessage + "\n";
            write(loggerFileDescriptor, reporterMessageWithEndLine.c_str(), reporterMessageWithEndLine.length());
            std::cout << reporterMessage << std::endl;
        }
    }
}

void LoggerServer::endLogger() {
    close(loggerFileDescriptor);
    logReader->deleteFifo();
    delete logReader;
}