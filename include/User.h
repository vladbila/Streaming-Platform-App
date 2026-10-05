#ifndef STREAMING_PLATFORM_PROJECT_USER_H
#define STREAMING_PLATFORM_PROJECT_USER_H

#include <string>
#include <vector>
#include <istream>
#include "IObject.h"
#include "Playlist.h"

class User : public IObject {
    std::string userName;
    char subscription;
    std::vector<Playlist*> playlists;
    std::vector<int> minutesListenedDaily;

    static const float subscriptionPrice;

public:
    User();
    User(const std::string& n, char s);
    User(const User &other);
    ~User() noexcept override;

    friend std::istream &operator>>(std::istream &is, User &u);

    User &operator=(const User &other);

    Playlist *operator[](int wantedIndex) const;

    std::string toString() const override;

    std::string getuserName() const { return userName; }
    int getnumberPlaylists() const { return playlists.size(); }
    char getsubscription() const { return subscription; }
    static float getSubscriptionPrice() { return subscriptionPrice; }

    void setuserName(const std::string& n);
    void setsubscription(char s);

    void showAllPlaylists() const;
    void showPlaylist() const;
    void addPlaylist();
    void addExistingPlaylist(Playlist* playlist);
    void recordMinutes(int minutes);
    void removePlaylist();
    void updatePlaylist();
    void addDailyMinutes();
    void showStats();
};

#endif