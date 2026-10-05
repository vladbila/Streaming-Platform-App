#ifndef STREAMING_PLATFORM_PROJECT_RELEASEDATE_H
#define STREAMING_PLATFORM_PROJECT_RELEASEDATE_H

#include <string>

class ReleaseDate {
    int day, month, year;

public:
    ReleaseDate(int d = 1, int m = 1, int y = 2000);
    ~ReleaseDate() noexcept = default;

    void readDate();
    std::string toString() const;
};

#endif