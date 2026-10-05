#include "Playlist.h"
#include "Song.h"
#include "Podcast.h"
#include "Exceptions.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <cctype>

Playlist::Playlist() : playlistName("Unknown Playlist"), privacy('N') {}

Playlist::Playlist(const std::string& n, char p) : playlistName(n), privacy(p) {}

void Playlist::setName(const std::string& n) {
    if (n.empty() || n.size() < 2) {
        throw InvalidDataException("The title of the playlist must contain more than 2 characters.", n.size());
    }
    playlistName = n;
}

void Playlist::setPrivacy(char p) {
    privacy = p;
}

void Playlist::addItem(MediaItem *itemToAdd) {
    if (itemToAdd == nullptr) {
        throw InvalidDataException("Can't add an invalid (null) item to the playlist!", -1);
    }
    for (MediaItem* currentItem : items) {
        if (*currentItem == *itemToAdd) {
            throw DuplicateItemException("This item already exists in the playlist!", itemToAdd->getId());
        }
    }
    items.push_back(itemToAdd);
    Logger::getInstance().logEvent("Item '" + itemToAdd->getTitle() + "' added to playlist: " + playlistName);
}

void Playlist::showAllItems() const {
    if (items.empty()) {
        std::cout << "[!] This playlist is empty." << std::endl;
        return;
    }
    for (MediaItem* item : items) {
        std::cout << *item << std::endl << std::endl;
    }
}

void Playlist::removeItem() {
    if (items.empty()) {
        std::cout << "[!] This playlist is empty." << std::endl;
        return;
    }
    int tempId;
    showAllItems();
    MediaItem *tempItem = nullptr;
    while (true) {
        std::cout << "* Enter the ID of the item you want to remove from this playlist: ";
        std::cin >> tempId;
        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a numeric ID.", -1);
            }
            tempItem = (*this)[tempId];
            if (!tempItem) {
                throw ItemNotFoundException("The ID you entered does not exist!", -1);
            }

            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "[!] Please try again." << std::endl << std::endl;
        } catch (const ItemNotFoundException &e) {
            std::cout << "[!] SEARCH ERROR: " << e.what() << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
        }
    }
    Logger::getInstance().logEvent("Item '" + tempItem->getTitle() + "' removed from playlist: " + playlistName);
    int idToErase = tempItem->getId();
    items.remove_if([idToErase](MediaItem* item) { return item->getId() == idToErase; });
    std::cout << "[-] The item has been removed." << std::endl;
}

void Playlist::orderSongs() {
    if (items.size() < 2)
        return;
    items.sort([](MediaItem* a, MediaItem* b) {
        Song *m1 = dynamic_cast<Song *>(a);
        Song *m2 = dynamic_cast<Song *>(b);
        if (m1 != nullptr && m2 != nullptr) {
            return *m2 < *m1;
        }
        return false;
    });
    std::cout << "=============== SORTED PLAYLIST ===============" << std::endl;
    int index = 1;
    for (MediaItem* item : items) {
        std::cout << index++ << "." << item->getTitle();

        Song *song = dynamic_cast<Song *>(item);
        if (song != nullptr) {
            std::cout << " by " << song->getArtist() << " (Rating: " << song->getRating() << ")" << std::endl;
        } else {
            Podcast *pod = dynamic_cast<Podcast *>(item);
            if (pod != nullptr) {
                std::cout << " (Podcast hosted by " << pod->getHost() << ")" << std::endl;
            }
        }
    }
}

std::istream &operator>>(std::istream &is, Playlist &p) {
    std::cout << "* Enter the name of the playlist: ";
    is.get();
    std::getline(is, p.playlistName);

    while (true) {
        std::cout << "* Will the playlist be private or public? (P for private and N for Public): ";
        is >> p.privacy;

        try {
            if (is.fail()) {
                is.clear();
                is.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a single letter.", -1);
            }

            p.privacy = std::toupper(p.privacy);

            if (p.privacy != 'P' && p.privacy != 'N') {
                is.ignore(256, '\n');
                throw InvalidDataException("The letter you entered is invalid. Please choose P or N.", -1);
            }

            is.ignore(256, '\n');
            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR!" << std::endl << std::endl;
        }
    }
    return is;
}

std::string Playlist::toString() const {
    std::ostringstream os;
    os << "=============== Playlist ID: " << getId() << " ===============" << std::endl;
    os << "* Playlist Name: " << playlistName << std::endl;
    os << "* Status: ";
    if (privacy == 'P')
        os << "Private" << std::endl;
    else
        os << "Public" << std::endl;
    os << "Songs:" << std::endl;
    if (items.empty()) {
        os << "[!] This playlist is empty." << std::endl;
    } else {
        int index = 1;
        for (MediaItem* item : items) {
            os << "     " << index++ << "." << item->getTitle();
            Song *song = dynamic_cast<Song *>(item);
            if (song != nullptr) {
                os << " by " << song->getArtist() << std::endl;
            } else {
                Podcast *pod = dynamic_cast<Podcast *>(item);
                if (pod != nullptr) {
                    os << " (Podcast hosted by " << pod->getHost() << ")" << std::endl;
                }
            }
        }
    }
    os << "=========================================================" << std::endl;
    return os.str();
}

MediaItem *Playlist::operator[](const int wantedId) const {
    for (MediaItem* item : items) {
        if (item->getId() == wantedId)
            return item;
    }
    return nullptr;
}