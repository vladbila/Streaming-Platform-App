#ifndef STREAMING_PLATFORM_PROJECT_REMASTERED_H
#define STREAMING_PLATFORM_PROJECT_REMASTERED_H

#include <string>
#include "Song.h"

class Remastered : public Song {
    std::string studio;
    int year;

public:
    Remastered(const std::string& t = "Unknown", const std::string& a = "N/A", float d = 0.0, float r = 0.0,
               const std::string& s = "Unknown Studio", int ry = 0, int day = 1, int month = 1, int year = 2000);

    ~Remastered() noexcept override = default;

    void readDetails() override;
    std::string toString() const override;
    void play() const override;

    std::string getStudio() const { return studio; }
};

#endif