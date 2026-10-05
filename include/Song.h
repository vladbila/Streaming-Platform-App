#ifndef STREAMING_PLATFORM_PROJECT_SONG_H
#define STREAMING_PLATFORM_PROJECT_SONG_H

#include <string>
#include "MediaItem.h"
#include "ReleaseDate.h"

class Song : public MediaItem {
protected:
    std::string artist;
    float rating;
    ReleaseDate launchDate;

public:
    Song();
    Song(const std::string& t, const std::string& a, float d, float r, int day, int m, int y);
    ~Song() noexcept override = default;

    void play() const override;

    std::string getArtist() const { return artist; }
    float getRating() const { return rating; }

    void setSongName(const std::string& newName);
    void setArtistName(const std::string& newName);
    void setLength(float length);
    void setRating(float newRating);

    void adjustDurationMinutes(int deltaMinutes);
    void adjustRating(float delta);

    static void validateLength(float length);

    bool operator==(const Song &m) const;
    bool operator<(const Song &m) const;
    bool operator<=(const Song &m) const;

    void readDetails() override;
    std::string toString() const override;
};

#endif