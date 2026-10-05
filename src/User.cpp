#include "User.h"
#include "Exceptions.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

const float User::subscriptionPrice = 4.99f;

User::User() : userName("User"), subscription('F') {}

User::User(const std::string& n, char s) : userName(n), subscription(s) {}

User::User(const User &other) : userName(other.userName), subscription(other.subscription) {
    minutesListenedDaily = other.minutesListenedDaily;
    for (Playlist* p : other.playlists) {
        playlists.push_back(new Playlist(*p));
    }
}

User::~User() noexcept {
    for (Playlist* p : playlists) {
        delete p;
    }
    playlists.clear();
}

User &User::operator=(const User &other) {
    if (this == &other)
        return *this;
    userName = other.userName;
    subscription = other.subscription;
    for (Playlist* p : playlists) {
        delete p;
    }
    playlists.clear();

    minutesListenedDaily = other.minutesListenedDaily;
    for (Playlist* p : other.playlists) {
        playlists.push_back(new Playlist(*p));
    }
    return *this;
}

Playlist *User::operator[](int wantedIndex) const {
    if (wantedIndex > 0 && wantedIndex <= playlists.size()) {
        return playlists[wantedIndex - 1];
    }
    return nullptr;
}

void User::setuserName(const std::string& n) {
    userName = n;
}

void User::setsubscription(char s) {
    subscription = s;
}

void User::showAllPlaylists() const {
    if (playlists.empty()) {
        std::cout << "[!] This user has no playlists created." << std::endl;
        return;
    }
    int index = 1;
    for (Playlist* p : playlists) {
        std::cout << index++ << "." << p->getPlaylistName() << std::endl;
    }
}

void User::showPlaylist() const {
    if (playlists.empty()) {
        std::cout << "[!] This user has no playlists created." << std::endl;
        return;
    }
    int tempIndex;
    this->showAllPlaylists();
    while (true) {
        std::cout << "* Enter the index of the playlist you want to see: ";
        std::cin >> tempIndex;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a number", -1);
            }
            Playlist *pRead = (*this)[tempIndex];
            if (!pRead)
                throw ItemNotFoundException("The index you entered is invalid (Playlist not found)!", tempIndex);
            std::cout << *pRead << std::endl;
            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "* Please try again!" << std::endl;
        } catch (const ItemNotFoundException &e) {
            std::cout << "[!] SEARCH ERROR: " << e.what() << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
        }
    }
}

void User::addPlaylist() {
    Playlist *newPlaylist = new Playlist;
    std::cin >> *newPlaylist;
    playlists.push_back(newPlaylist);

    Logger::getInstance().logEvent("User " + userName + " created playlist: " + newPlaylist->getPlaylistName());
    std::cout << "[+] Playlist " << newPlaylist->getPlaylistName() << " has been created for user " << userName << "." << std::endl;
}

void User::addExistingPlaylist(Playlist* playlist) {
    if (playlist) {
        playlists.push_back(playlist);
    }
}

void User::recordMinutes(int minutes) {
    if (minutes >= 0) {
        minutesListenedDaily.push_back(minutes);
        Logger::getInstance().logEvent("User '" + userName + "' logged " + std::to_string(minutes) + " listening minutes.");
    }
}

void User::removePlaylist() {
    if (playlists.empty()) {
        std::cout << "[!] This user has no playlists created." << std::endl;
        return;
    }
    int tempIndex;
    this->showAllPlaylists();

    Playlist *pToRemove = nullptr;
    while (true) {
        std::cout << "* Enter the index of the playlist you want to remove: ";
        std::cin >> tempIndex;
        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a number", -1);
            }
            pToRemove = (*this)[tempIndex];
            if (!pToRemove) {
                throw ItemNotFoundException("The index you entered is invalid!", -1);
            }
            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "* Please try again!" << std::endl;
        } catch (const ItemNotFoundException &e) {
            std::cout << "[!] SEARCH ERROR: " << e.what() << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
        }
    }
    Logger::getInstance().logEvent("Playlist '" + pToRemove->getPlaylistName() + "' has been deleted from user: " + this->userName);
    int idToErase = pToRemove->getId();
    playlists.erase(std::remove_if(playlists.begin(), playlists.end(), [idToErase](Playlist *p) {
        return p->getId() == idToErase;
    }), playlists.end());

    delete pToRemove;

    std::cout << "[-] The playlist has been removed." << std::endl << std::endl;
}

void User::updatePlaylist() {
    if (playlists.empty()) {
        std::cout << "[!] This user has no playlists created." << std::endl;
        return;
    }
    int tempIndex;
    this->showAllPlaylists();

    Playlist *pToUpdate = nullptr;

    while (true) {
        std::cout << "* Enter the index of the playlist you want to update: ";
        std::cin >> tempIndex;
        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a number", -1);
            }
            pToUpdate = (*this)[tempIndex];
            if (!pToUpdate) {
                throw ItemNotFoundException("The index you entered is invalid!", tempIndex);
            }
            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "* Please try again!" << std::endl;
        } catch (const ItemNotFoundException &e) {
            std::cout << "[!] SEARCH ERROR: " << e.what() << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
        }
    }
    int tempOption;
    std::cout << "================ Options =================" << std::endl;
    std::cout << "PLAYLIST: " << pToUpdate->getPlaylistName() << std::endl;
    std::cout << "1. Change the name of the playlist." << std::endl;
    std::cout << "2. Change the status of the playlist." << std::endl;
    std::cout << "3. Remove an item from the playlist." << std::endl;
    std::cout << "4. Organize the tracks by rating." << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Choose an option(1-4): ";
    std::cin >> tempOption;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(256, '\n');
        tempOption = -1;
    }

    switch (tempOption) {
        case 1: {
            std::string oldName = pToUpdate->getPlaylistName();
            std::string newName;
            std::cout << "* Enter the new name of the playlist: ";
            std::cin.get();
            std::getline(std::cin, newName);
            try {
                pToUpdate->setName(newName);
                Logger::getInstance().logEvent("Playlist '" + oldName + "' renamed to '" + newName + "'.");
            } catch (const InvalidDataException &e) {
                std::cout << "[!] ERROR: " << e.what() << std::endl;
            } catch (const Exceptions& e) {
                std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
            } catch (...) {
                std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
            }
            break;
        }
        case 2: {
            if (pToUpdate->getPrivacy() == 'P')
                pToUpdate->setPrivacy('N');
            else
                pToUpdate->setPrivacy('P');
            Logger::getInstance().logEvent("Status of playlist '" + pToUpdate->getPlaylistName() + "' has been changed to '" + std::string(1, pToUpdate->getPrivacy()) + "'.");
            std::cout << "[!] The status of the playlist has been changed." << std::endl;

            break;
        }
        case 3: {
            pToUpdate->removeItem();
            break;
        }
        case 4: {
            pToUpdate->orderSongs();
            break;
        }
        default:
            std::cout << "[!] Invalid option, we will return you to the main menu." << std::endl;
    }
}

void User::addDailyMinutes() {
    int minutes;
    while (true) {
        std::cout << "* Enter the number of minutes for today: ";
        std::cin >> minutes;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a number", -1);
            }
            if (minutes < 0) {
                throw InvalidDataException("The minutes cannot be negative!", static_cast<float>(minutes));
            }
            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "* Please try again!" << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
        }
    }
    recordMinutes(minutes);

    std::cout << "[+] " << minutes << " minutes have been added to your daily stats!" << std::endl;
}

void User::showStats() {
    if (minutesListenedDaily.empty()) {
        std::cout << "[!] There are no stats registered for this user." << std::endl;
        return;
    }
    std::cout << "==================== Stats ====================" << std::endl;
    int totalMinutes = 0;
    int maxMinutes = minutesListenedDaily[0];

    for (int mins : minutesListenedDaily) {
        totalMinutes += mins;
        if (mins > maxMinutes) {
            maxMinutes = mins;
        }
    }

    std::cout << "[=] Number of days active in app: " << minutesListenedDaily.size() << std::endl;
    std::cout << "[=] Total minutes listened: " << totalMinutes << std::endl;
    std::cout << "[=] Your record in a single day: " << maxMinutes << std::endl;
    int activeDays = std::count_if(minutesListenedDaily.begin(), minutesListenedDaily.end(), [](int mins) {
        return mins >= 60;
    });
    std::cout << "[=] Days with more than 1 hour of listening: " << activeDays << std::endl;
    std::cout << "===============================================" << std::endl << std::endl;
}

std::istream &operator>>(std::istream &is, User &u) {
    std::cout << "* Enter the name of the User: ";
    is.get();
    std::getline(is, u.userName);

    while (true) {
        std::cout << "* Select the subscription of the user (F for Free / P for Premium): ";
        is >> u.subscription;

        try {
            if (is.fail()) {
                is.clear();
                is.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a single letter.", -1);
            }

            u.subscription = std::toupper(u.subscription);

            if (u.subscription != 'P' && u.subscription != 'F') {
                is.ignore(256, '\n');
                throw InvalidDataException("The letter you entered is invalid. Please choose F or P.", -1);
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

std::string User::toString() const {
    std::ostringstream os;
    os << "================= User ID: " << getId() << " =================" << std::endl;
    os << "* User Name: " << userName << std::endl;
    os << "* User Subscription: ";
    if (subscription == 'P') {
        os << "Premium ($" << std::fixed << std::setprecision(2) << subscriptionPrice << "/month)" << std::endl;
    } else os << "Free ($0.00/month)" << std::endl;
    os << "* User's Playlists: " << std::endl;
    if (playlists.empty()) {
        os << "[!] This user has no playlists." << std::endl;
    } else {
        int index = 1;
        for (Playlist* p : playlists) {
            os << "     " << index++ << "." << p->getPlaylistName() << " (ID: " << p->getId() << ")" << std::endl;
        }
    }
    os << "==================================================";
    return os.str();
}