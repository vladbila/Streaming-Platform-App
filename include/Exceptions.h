#ifndef STREAMING_PLATFORM_PROJECT_EXCEPTIONS_H
#define STREAMING_PLATFORM_PROJECT_EXCEPTIONS_H
#include <exception>
#include <string>

class Exceptions : public std::exception {
protected:
    std::string message;

public:
    explicit Exceptions(const std::string &m) : message(m) {
    }

    const char *what() const noexcept override { return message.c_str(); }
};

class InvalidDataException : public Exceptions {
    float invalidValue;

public:
    InvalidDataException(const std::string &m, float v) : Exceptions(
                                                              m + " (Received Value: " + std::to_string(v) + ")"),
                                                          invalidValue(v) {
    }

    float getInvalidValue() const { return invalidValue; }

};

class ItemNotFoundException : public Exceptions {
    int missingId;

public:
    ItemNotFoundException(const std::string &m, int id) : Exceptions(m + " (Wanted ID: " + std::to_string(id) + ")"),
                                                          missingId(id) {
    }

    int getmissingId() const { return missingId; }
};

class DuplicateItemException : public Exceptions {
    int duplicateId;

public:
    DuplicateItemException(const std::string &m, int id) : Exceptions(
                                                               m + " (Duplicate ID: " + std::to_string(id) + ")"),
                                                           duplicateId(id) {
    }

    int getDuplicateId() const { return duplicateId; }
};


#endif