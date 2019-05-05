#include <utility>

#include "Logger.h"
#include <ctime>
#include <fcntl.h>
#include <unistd.h>


static const char LOG_FILE[] = "logs/log.txt";

Logger *Logger::instance = nullptr;
LogLevel Logger::runningLogLevel = INFO;
int Logger::fd = 0;

Logger *Logger::getInstance(std::string reporter) {
    if (instance == nullptr) {
        instance = new Logger();
        fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0666);
    }
    instance->reporter = std::move(reporter);
    return instance;
}

void Logger::setLogLevel(LogLevel logLevel) {
    runningLogLevel = logLevel;
}

void Logger::logMessage(LogLevel logLevel, std::string message) {
    if (runningLogLevel <= logLevel) {
        auto now = time(0);
        auto string_time = std::string(ctime(&now));
        string_time.resize(string_time.length()-1);
        std::string reporterMessage = string_time + " :: " + logLevelNames[logLevel] + " :: " + reporter +
                " :: " + message;
        std::string reporterMessageWithEndLine = reporterMessage + "\n";
        write(fd, reporterMessageWithEndLine.c_str(), reporterMessageWithEndLine.length());
        std::cout << reporterMessage << std::endl;
    }
}

void Logger::endLogger() {
    close(fd);
}