#ifndef STREAMING_PLATFORM_PROJECT_MEDIAITEM_H
#define STREAMING_PLATFORM_PROJECT_MEDIAITEM_H

#include <string>
#include "IObject.h"

class MediaItem : public IObject {
protected:
    std::string title;
    float duration;

public:
    MediaItem(const std::string& t, const float d) : title(t), duration(d) {}

    ~MediaItem() override = default;

    virtual void play() const = 0;
    virtual void readDetails() = 0;

    std::string getTitle() const { return title; }
    float getDuration() const { return duration; }
};

#endif