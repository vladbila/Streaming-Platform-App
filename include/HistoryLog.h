#ifndef STREAMING_PLATFORM_PROJECT_HISTORYLOG_H
#define STREAMING_PLATFORM_PROJECT_HISTORYLOG_H

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cstddef>

template <typename T>
void notifyPlatform(const T& message) {
    std::cout << ">>> [PLATFORM NOTIFICATION]: " << message << std::endl;
}

template <>
inline void notifyPlatform<std::string>(const std::string& message) {
    std::cout << ">>> [ALERT]: *** " << message << " ***" << std::endl;
}

template <typename T, std::size_t MaxCapacity>
class HistoryLog {
    std::vector<T> elements;
public:
    HistoryLog() {
        elements.reserve(MaxCapacity);
    }

    void addAction(const T& item) {
        if (elements.size() >= MaxCapacity) {
            throw std::overflow_error("The history log is full!");
        }
        elements.push_back(item);
    }

    T undoAction() {
        if (elements.empty()) {
            throw std::underflow_error("The history log is empty! Nothing to undo");
        }
        T lastItem = elements.back();
        elements.pop_back();
        return lastItem;
    }

    template <typename U>
    void exportToExternal(std::vector<U>& externalContainer) const {
        for (const auto& elem : elements) {
            externalContainer.push_back(static_cast<U>(elem));
        }
    }

    void printLog() const {
        if (elements.empty()) {
            std::cout << "[!] History is empty." << std::endl;
            return;
        }
        for (const auto& elem : elements) {
            std::cout << " - " << elem << std::endl;
        }
    }
};

#endif