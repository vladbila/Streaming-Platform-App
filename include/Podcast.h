#ifndef STREAMING_PLATFORM_PROJECT_PODCAST_H
#define STREAMING_PLATFORM_PROJECT_PODCAST_H

#include <string>
#include "MediaItem.h"

class Podcast : public MediaItem {
    std::string host;

public:
    Podcast(const std::string& t = "Unknown", float d = 0.0, const std::string& h = "Unknown Host");
    ~Podcast() noexcept override = default;

    void play() const override;
    std::string toString() const override;
    void readDetails() override;

    std::string getHost() const { return host; }
};

#endif