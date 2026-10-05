#ifndef STREAMING_PLATFORM_PROJECT_MUSICPLATFORM_H
#define STREAMING_PLATFORM_PROJECT_MUSICPLATFORM_H

#include <string>
#include <map>
#include <vector>
#include "User.h"
#include "Artist.h"
#include "Podcast.h"
#include "HistoryLog.h"

class MusicPlatform {
    std::string name;
    int premiumUsers;
    int option;
    std::map<int, User*> users;
    std::map<int, Artist*> artists;
    std::vector<Podcast*> globalPodcasts;

    HistoryLog<std::string, 10> actionHistory;

public:
    explicit MusicPlatform(const std::string& n);
    ~MusicPlatform() noexcept;

    User *getUser(int wantedID) const;
    Podcast *getPodcast(int wantedID) const;
    Artist *getArtist(int wantedID) const;

    void addGlobalPodcast();
    void showAllPodcasts();

    void showAllUsers() const;
    void showUser() const;
    void addUser();
    void deleteUser();
    void updateUser();

    void showAllArtists() const;
    void showGlobalRanking() const;
    void showArtist() const;
    void addArtist();
    void addSongToPlaylist(const Artist *artist);
    void updateArtist();

    void showMenu();
    int getOption() const { return option; }
    void seedDemoData();
};

#endif