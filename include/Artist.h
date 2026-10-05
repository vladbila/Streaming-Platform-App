#ifndef STREAMING_PLATFORM_PROJECT_ARTIST_H
#define STREAMING_PLATFORM_PROJECT_ARTIST_H

#include <string>
#include <vector>
#include "IObject.h"
#include "Song.h"

class Artist : public IObject {
    std::string name;
    int monListeners;
    int DebutYear;

    std::vector<Song*> discography;

public:
    Artist();
    Artist(const std::string& name, int monListeners, int DebutYear);
    Artist(const Artist &other);
    ~Artist() noexcept override;

    Artist &operator=(const Artist &other);

    friend std::istream &operator>>(std::istream &is, Artist &a);

    std::string toString() const override;

    std::string getName() const { return name; }
    int getMonListeners() const { return monListeners; }
    int getDebutYear() const { return DebutYear; }
    int getSongs() const { return discography.size(); }

    void setName(const std::string& newName);
    void setMonListeners(int newMonListeners);
    void adjustMonthlyListeners(int delta);

    bool operator==(const Artist &other) const;
    bool operator<(const Artist &other) const;
    bool operator<=(const Artist &other) const;
    bool operator>(const Artist &other) const;
    bool operator>=(const Artist &other) const;

    Song *operator[](int wantedId) const;

    void addSong();
    void addRemasteredSong();
    void addExistingSong(Song* song);
    void readAllSongs() const;
    void readSong() const;
    void updateSong();
};

#endif