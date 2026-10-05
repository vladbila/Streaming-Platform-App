#ifndef STREAMING_PLATFORM_PROJECT_PLAYLIST_H
#define STREAMING_PLATFORM_PROJECT_PLAYLIST_H

#include <string>
#include <list>
#include <istream>
#include "IObject.h"
#include "MediaItem.h"

class Playlist : public IObject {
    std::string playlistName;
    std::list<MediaItem*> items;
    char privacy;

public:
    Playlist();
    Playlist(const std::string& n, char p);
    ~Playlist() noexcept override = default;

    std::string toString() const override;

    friend std::istream &operator>>(std::istream &is, Playlist &p);

    MediaItem *operator[](int wantedId) const;

    std::string getPlaylistName() const { return playlistName; }
    int getPlaylistSize() const { return items.size(); }
    char getPrivacy() const { return privacy; }

    void setName(const std::string& n);
    void setPrivacy(char p);

    void addItem(MediaItem *itemToAdd);
    void showAllItems() const;
    void removeItem();
    void orderSongs();
};

#endif