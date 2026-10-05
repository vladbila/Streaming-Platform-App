#ifndef STREAMING_PLATFORM_PROJECT_LOGGER_H
#define STREAMING_PLATFORM_PROJECT_LOGGER_H
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>

class Logger {
    std::ofstream logFile;

    Logger() {
        logFile.open("system_log.txt", std::ios::app);
    }

    ~Logger() noexcept {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

public:
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void logEvent(const std::string& message) {
        if (logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
            std::tm localTm = *std::localtime(&nowTime);
            logFile << "[" << std::put_time(&localTm, "%Y-%m-%d %H:%M:%S") << "] [SYSTEM LOG] " << message << std::endl;
        }
    }
};

#endif //STREAMING_PLATFORM_PROJECT_LOGGER_H