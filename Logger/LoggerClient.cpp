#include <utility>

#include <utility>

#include "LoggerClient.h"
#include <ctime>
#include <fcntl.h>
#include <unistd.h>


FifoWriter *LoggerClient::logWriter = nullptr;

LoggerClient::LoggerClient(std::string reporter) {
    this->reporter = std::move(reporter);
    if (logWriter == nullptr) {
        logWriter = new FifoWriter(LOG_FIFO_PATH);
        logWriter->open_fifo();
    }
}

void LoggerClient::logMessage(LogLevel logLevel, const std::string& message) {
        auto now = time(nullptr);
        auto string_time = std::string(ctime(&now));
        string_time.resize(string_time.length()-1);
        LogMessage logMessageT{};
        logMessageT.logLevel = logLevel;
        strcpy(logMessageT.reporter, reporter.c_str());
        strcpy(logMessageT.logText, message.c_str());
        strcpy(logMessageT.timeStamp, string_time.c_str());
        int buffer_size = 281;
        char buffer[buffer_size];
        logMessageT.toString(buffer);
        logWriter->write_fifo(static_cast<const void *>(buffer), buffer_size);
}


LoggerClient::~LoggerClient() {}
