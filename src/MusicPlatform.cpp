#include "MusicPlatform.h"
#include "Remastered.h"
#include "Exceptions.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

MusicPlatform::MusicPlatform(const std::string& n) : name(n), premiumUsers(0), option(-1) {
    Logger::getInstance().logEvent("Platform '" + name + "' initialized successfully.");
}

MusicPlatform::~MusicPlatform() noexcept {
    for (auto [id, userPtr] : users) {
        delete userPtr;
    }
    users.clear();
    for (auto [id, artistPtr] : artists) {
        delete artistPtr;
    }
    artists.clear();
    for (Podcast* p : globalPodcasts)
        delete p;
    globalPodcasts.clear();

    Logger::getInstance().logEvent("Platform '" + name + "' shutting down. Memory cleared.");
}

User *MusicPlatform::getUser(int wantedID) const {
    auto it = users.find(wantedID);
    if (it != users.end()) {
        return it->second;
    }
    return nullptr;
}

Podcast *MusicPlatform::getPodcast(int wantedID) const {
    auto it = std::find_if(globalPodcasts.begin(), globalPodcasts.end(), [wantedID](Podcast* p) {
        return p->getId() == wantedID;
    });
    if (it != globalPodcasts.end()) {
        return *it;
    }
    return nullptr;
}

Artist *MusicPlatform::getArtist(int wantedID) const {
    auto it = artists.find(wantedID);
    if (it != artists.end()) {
        return it->second;
    }
    return nullptr;
}

void MusicPlatform::addGlobalPodcast() {
    Podcast *newPodcast = new Podcast();
    newPodcast->readDetails();

    globalPodcasts.push_back(newPodcast);
    Logger::getInstance().logEvent("New podcast created: '" + newPodcast->getTitle() + "' hosted by " + newPodcast->getHost());
    std::cout << "[+] Podcast '" << newPodcast->getTitle() << "' has been created." << std::endl;
}

void MusicPlatform::showAllPodcasts() {
    if (globalPodcasts.empty()) {
        std::cout << "[!] There are no podcasts created yet." << std::endl;
        return;
    }

    std::sort(globalPodcasts.begin(), globalPodcasts.end(), [](Podcast* a, Podcast* b) {
        return a->getDuration() < b->getDuration();
    });
    std::cout << "\n=============== PODCASTS ====================" << std::endl << std::endl;
    std::for_each(globalPodcasts.begin(), globalPodcasts.end(), [](Podcast* p) {
        std::cout << *p << std::endl << std::endl;
    });
    std::cout << "===============================================" << std::endl << std::endl;
}

void MusicPlatform::showAllUsers() const {
    if (users.empty()) {
        std::cout << "[!] There are no users created." << std::endl;
        return;
    }
    int index = 1;
    for (const auto& [id, userPtr] : users) {
        std::cout << index++ << "." << userPtr->getuserName() << "(ID: " << id << ")" << std::endl;
    }
}

void MusicPlatform::showUser() const {
    if (users.empty()) {
        std::cout << "[!] There are no users created." << std::endl;
        return;
    }
    int tempId;
    this->showAllUsers();
    while (true) {
        std::cout << "* Enter the ID of the User you want to see: ";
        std::cin >> tempId;
        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a number!", static_cast<float>(tempId));
            }
            User *userRead = getUser(tempId);
            if (!userRead) {
                throw ItemNotFoundException("The ID you entered is invalid! (User not found)", tempId);
            }
            std::cout << *userRead;
            std::cout << std::endl;
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

void MusicPlatform::addUser() {
    User *newUser = new User;
    std::cin >> *newUser;
    users[newUser->getId()] = newUser;
    if (newUser->getsubscription() == 'P') {
        premiumUsers++;
    }
    std::string msg = "[!] User " + newUser->getuserName() + " has been created.";
    notifyPlatform(msg);
    Logger::getInstance().logEvent("New user account created: " + newUser->getuserName() + " (ID: " + std::to_string(newUser->getId()) + ")");
    try {
        actionHistory.addAction("Added user: " + newUser->getuserName());
    } catch (const std::overflow_error& e) {
        std::cout << "[!] HISTORY ERROR: " << e.what() << std::endl;
    }
}

void MusicPlatform::deleteUser() {
    if (users.empty()) {
        std::cout << "[!] There are no users created." << std::endl;
        return;
    }
    int tempId;
    this->showAllUsers();

    User *userToDelete = nullptr;
    while (true) {
        std::cout << "* Enter the ID of the user you want to delete: ";
        std::cin >> tempId;
        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a number!", static_cast<float>(tempId));
            }
            userToDelete = getUser(tempId);
            if (!userToDelete) {
                throw ItemNotFoundException("The ID you entered is invalid!", tempId);
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
    if (userToDelete->getsubscription() == 'P') {
        premiumUsers--;
    }
    users.erase(userToDelete->getId());
    Logger::getInstance().logEvent("User account deleted: " + userToDelete->getuserName() + " (ID: " + std::to_string(userToDelete->getId()) + ")");
    delete userToDelete;

    std::cout << "* The user has been deleted." << std::endl;
}

void MusicPlatform::updateUser() {
    if (users.empty()) {
        std::cout << "[!] There are no users created." << std::endl;
        return;
    }
    int tempId;
    this->showAllUsers();

    User *userToUpdate = nullptr;
    while (true) {
        std::cout << "* Enter the ID of the user you want to update: ";
        std::cin >> tempId;
        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a number!", static_cast<float>(tempId));
            }
            userToUpdate = getUser(tempId);
            if (!userToUpdate) {
                throw ItemNotFoundException("The ID you entered is invalid!", tempId);
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
    std::cout << "================== Options ==================" << std::endl;
    std::cout << "USER: " << userToUpdate->getuserName() << "(ID: " << userToUpdate->getId() << ")" << std::endl;
    std::cout << "1. Change the name of the user." << std::endl;
    std::cout << "2. Change the subscription of the user" << std::endl;
    std::cout << "3. Create a playlist." << std::endl;
    std::cout << "4. Delete a playlist." << std::endl;
    std::cout << "5. Modify a playlist." << std::endl;
    std::cout << "6. Show all the playlists this user has created." << std::endl;
    std::cout << "7. Show the details and songs of a playlist." << std::endl;
    std::cout << "8. Add minutes listened in a day." << std::endl;
    std::cout << "9. Show registered stats." << std::endl;
    std::cout << "10. Play a song/podcast from a playlist." << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "Choose an option(1-10): ";
    std::cin >> tempOption;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(256, '\n');
        tempOption = -1;
    }

    switch (tempOption) {
        case 1: {
            std::string oldName = userToUpdate->getuserName();
            std::string newName;
            std::cout << "* Enter the new name of the User: ";
            std::cin.get();
            std::getline(std::cin, newName);
            userToUpdate->setuserName(newName);
            Logger::getInstance().logEvent("The User '" + oldName + "' has been renamed to '" + newName + "'.");
            std::cout << "[!] The name of the user has been changed." << std::endl;
            break;
        }
        case 2: {
            if (userToUpdate->getsubscription() == 'P') {
                userToUpdate->setsubscription('F');
                premiumUsers--;
                Logger::getInstance().logEvent("The subscription of the user '" + userToUpdate->getuserName() + "' has been changed to Free.");
            } else {
                userToUpdate->setsubscription('P');
                premiumUsers++;
                Logger::getInstance().logEvent("The subscription of the user '" + userToUpdate->getuserName() + "' has been changed to Premium.");
            }
            std::cout << "[!] The subscription of the user has been changed." << std::endl;

            break;
        }
        case 3: {
            userToUpdate->addPlaylist();
            break;
        }
        case 4: {
            userToUpdate->removePlaylist();
            break;
        }
        case 5: {
            userToUpdate->updatePlaylist();
            break;
        }
        case 6: {
            userToUpdate->showAllPlaylists();
            break;
        }
        case 7: {
            userToUpdate->showPlaylist();
            break;
        }
        case 8: {
            userToUpdate->addDailyMinutes();
            break;
        }
        case 9: {
            userToUpdate->showStats();
            break;
        }
        case 10: {
            if (userToUpdate->getnumberPlaylists() == 0) {
                std::cout << "[!] This user has no playlists created." << std::endl;
                return;
            }
            userToUpdate->showAllPlaylists();
            int tempIndex;
            Playlist* pPlay = nullptr;

            while (true) {
                std::cout << "* Enter the index of the playlist you want to listen to: ";
                std::cin >> tempIndex;
                try {
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(256, '\n');
                        throw InvalidDataException("Invalid input! Please enter a number", tempIndex);
                    }
                    pPlay = (*userToUpdate)[tempIndex];
                    if (!pPlay) {
                        throw ItemNotFoundException("Item not found! The index you entered is invalid!", tempIndex);
                    }
                    break;
                } catch (const InvalidDataException &e) {
                    std::cout << "[!] ERROR: " << e.what() << std::endl;
                    std::cout << "* Please try again!" << std::endl;
                } catch (const ItemNotFoundException &e) {
                    std::cout << "[!] SEARCH ERROR: " << e.what() << std::endl;
                    std::cout << "* Please try again!" << std::endl;
                } catch (const Exceptions &e) {
                    std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
                } catch (...) {
                    std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
                }
            }
            if (pPlay->getPlaylistSize() == 0) {
                std::cout << "[!] This playlist is empty! Please add items to it!" << std::endl;
                break;
            }
            pPlay->showAllItems();
            int tempIdMedia;
            MediaItem* itemToPlay = nullptr;

            while (true) {
                std::cout << "* Enter the ID of the song/podcast you want to play: ";
                std::cin >> tempIdMedia;
                try {
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(256, '\n');
                        throw InvalidDataException("Invalid input! Please enter a number", tempIdMedia);
                    }
                    itemToPlay = (*pPlay)[tempIdMedia];
                    if (!itemToPlay) {
                        throw ItemNotFoundException("The ID you entered was not found in this playlist.", tempIdMedia);
                    }
                    break;
                } catch (const InvalidDataException &e) {
                    std::cout << "[!] ERROR: " << e.what() << std::endl;
                    std::cout << "* Please try again!" << std::endl;
                } catch (const ItemNotFoundException &e) {
                    std::cout << "[!] SEARCH ERROR: " << e.what() << std::endl;
                    std::cout << "* Please try again!" << std::endl;
                } catch (const Exceptions &e) {
                    std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
                } catch (...) {
                    std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
                }
            }

            std::cout << std::endl;
            itemToPlay->play();
            Logger::getInstance().logEvent("User '" + userToUpdate->getuserName() + "' played '" + itemToPlay->getTitle() + "'.");
            std::cout << std::endl;

            try {
                actionHistory.addAction("Audio playback: " + itemToPlay->getTitle() + " (User: " + userToUpdate->getuserName() + ")");
            } catch (const std::overflow_error& e) {
                std::cout << "[!] HISTORY ERROR: " << e.what() << std::endl;
            }

            break;
        }
        default: {
            std::cout << "[!] Invalid option, we will return you to the main menu." << std::endl;
        }
    }
}

void MusicPlatform::showAllArtists() const {
    if (artists.empty()) {
        std::cout << "[!] There are no artists created." << std::endl;
        return;
    }
    for (const auto& [id, artistPtr] : artists) {
        std::cout << "* " << artistPtr->getName() << "(ID: " << id << ")" << std::endl;
    }
}

void MusicPlatform::showGlobalRanking() const {
    if (artists.empty()) {
        std::cout << "[!] There are no artists on the platform to rank." << std::endl;
        return;
    }
    std::vector<Artist*> ranking;
    for (const auto& [id, artistPtr] : artists) {
        if (artistPtr != nullptr)
            ranking.push_back(artistPtr);
    }

    if (ranking.size() < 2) {
        std::cout << "[!] Not enough artists to perform a ranking." << std::endl;
        return;
    }

    std::sort(ranking.begin(), ranking.end(), [](const Artist* a, const Artist* b) {
        return *a > *b;
    });

    std::cout << std::endl << "==================== GLOBAL ARTIST RANKING ====================" << std::endl;
    int rank = 1;
    for (const Artist* a : ranking) {
        std::cout << "  #" << rank++ << ". " << a->getName() << " - " << a->getMonListeners() << " monthly listeners" << std::endl;
    }
    std::cout << "===============================================================" << std::endl << std::endl;
}

void MusicPlatform::showArtist() const {
    if (artists.empty()) {
        std::cout << "[!] There are no artists created." << std::endl;
        return;
    }
    int tempId;
    this->showAllArtists();
    while (true) {
        std::cout << "* Enter the ID of the Artist you want to see: ";
        std::cin >> tempId;
        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a number!", static_cast<float>(tempId));
            }
            const Artist *artistRead = getArtist(tempId);
            if (!artistRead) {
                throw ItemNotFoundException("The ID you entered is invalid!", tempId);
            }
            std::cout << *artistRead << std::endl;
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

void MusicPlatform::addArtist() {
    Artist *newArtist = new Artist;
    std::cin >> *newArtist;
    artists[newArtist->getId()] = newArtist;
    std::string msg = "[!] Artist " + newArtist->getName() + " has been created.";
    notifyPlatform(msg);
    Logger::getInstance().logEvent("New artist profile created: " + newArtist->getName() + " (ID: " + std::to_string(newArtist->getId()) + ")");
    try {
        actionHistory.addAction("Added artist: " + newArtist->getName());
    } catch (const std::overflow_error& e) {
        std::cout << "[!] HISTORY ERROR: " << e.what() << std::endl;
    }
}

void MusicPlatform::addSongToPlaylist(const Artist *artist) {
    if (artist->getSongs() == 0) {
        std::cout << "[!] The artist hasn't released any songs." << std::endl;
        return;
    }
    int tempIdsong;
    artist->readAllSongs();

    Song *songToAdd = nullptr;
    while (true) {
        std::cout << "* Enter the ID of the song you want to add: ";
        std::cin >> tempIdsong;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a numeric ID.", tempIdsong);
            }
            songToAdd = (*artist)[tempIdsong];
            if (!songToAdd) {
                throw ItemNotFoundException("The ID you entered is invalid.", tempIdsong);
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
    if (users.empty()) {
        std::cout << "[!] There are no users created." << std::endl;
        return;
    }
    showAllUsers();
    int tempIdUser;

    User *tempUser = nullptr;
    while (true) {
        std::cout << "* Enter the ID of the User: ";
        std::cin >> tempIdUser;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a numeric ID.", static_cast<float>(tempIdUser));
            }
            tempUser = getUser(tempIdUser);
            if (!tempUser) {
                throw ItemNotFoundException("The ID you entered is invalid.", tempIdUser);
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
    if (tempUser->getnumberPlaylists() == 0) {
        std::cout << "[!] There are no playlists created." << std::endl;
        return;
    }
    tempUser->showAllPlaylists();
    int tempIndex;

    Playlist *tempPlaylist = nullptr;
    while (true) {
        std::cout << "* Enter the index of the playlist: ";
        std::cin >> tempIndex;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a numeric index.",
                                           static_cast<float>(tempIndex));
            }
            tempPlaylist = (*tempUser)[tempIndex];
            if (!tempPlaylist) {
                throw ItemNotFoundException("The index you selected is invalid.", tempIndex);
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
    try {
        tempPlaylist->addItem(songToAdd);
        std::cout << "[+] Item '" << songToAdd->getTitle() << "' has been added to playlist " << tempPlaylist->getPlaylistName() << "." << std::endl;
    } catch (const DuplicateItemException &e) {
        std::cout << "[!] DUPLICATE ERROR: " << e.what() << std::endl;
    } catch (const InvalidDataException &e) {
        std::cout << "[!] ERROR: " << e.what() << std::endl;
    } catch (const Exceptions &e) {
        std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
    }
}

void MusicPlatform::updateArtist() {
    if (artists.empty()) {
        std::cout << "[!] There are no artists created." << std::endl;
        return;
    }
    int tempId;
    this->showAllArtists();

    Artist *artistToUpdate = nullptr;
    while (true) {
        std::cout << "* Enter the ID of the artist: ";
        std::cin >> tempId;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a numeric ID.", static_cast<float>(tempId));
            }

            artistToUpdate = getArtist(tempId);
            if (!artistToUpdate) {
                throw ItemNotFoundException("The ID you entered is invalid.", tempId);
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
    int tempOption;
    std::cout << "================== Options ==================" << std::endl;
    std::cout << "ARTIST: " << artistToUpdate->getName() << "(ID: " << artistToUpdate->getId() << ")" << std::endl;
    std::cout << "1. Change the name of the artist" << std::endl;
    std::cout << "2. Change the monthly listeners of the artist" << std::endl;
    std::cout << "3. Add 1000 monthly listeners" << std::endl;
    std::cout << "4. Remove 1000 monthly listeners" << std::endl;
    std::cout << "5. Add a song" << std::endl;
    std::cout << "6. Add a remastered song" << std::endl;
    std::cout << "7. Modify a song" << std::endl;
    std::cout << "8. Show all songs the artist has released" << std::endl;
    std::cout << "9. Show the details of a song" << std::endl;
    std::cout << "10. Add a song to a user's playlist." << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "Choose an option(1-10): ";
    std::cin >> tempOption;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(256, '\n');
        tempOption = -1;
    }

    switch (tempOption) {
        case 1: {
            std::string newName;
            std::cout << "* Enter the new name of the artist: ";
            std::cin.get();
            std::getline(std::cin, newName);
            try {
                artistToUpdate->setName(newName);
                Logger::getInstance().logEvent("Artist renamed to '" + newName + "'.");
                std::cout << "[!] The name of the artist has been changed." << std::endl;
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
            int monListeners;
            while (true) {
                std::cout << "* Enter the new number of monthly listeners of the artist: ";
                std::cin >> monListeners;
                try {
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(256, '\n');
                        throw InvalidDataException("Invalid input! Please enter a valid number!",
                                                   static_cast<float>(monListeners));
                    }
                    artistToUpdate->setMonListeners(monListeners);
                    Logger::getInstance().logEvent("Updated monthly listeners for artist '" + artistToUpdate->getName() + "' to " + std::to_string(artistToUpdate->getMonListeners()) + ".");
                    std::cout << "[!] Listeners updated successfully." << std::endl;
                    break;
                } catch (const InvalidDataException &e) {
                    std::cout << "[!] ERROR: " << e.what() << std::endl;
                    std::cout << "[!] Please try again." << std::endl << std::endl;
                } catch (const Exceptions &e) {
                    std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
                } catch (...) {
                    std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
                }
            }
            break;
        }
        case 3: {
            artistToUpdate->adjustMonthlyListeners(1000);
            Logger::getInstance().logEvent("Updated monthly listeners for artist '" + artistToUpdate->getName() + "' to " + std::to_string(artistToUpdate->getMonListeners()) + ".");
            break;
        }
        case 4: {
            artistToUpdate->adjustMonthlyListeners(-1000);
            Logger::getInstance().logEvent("Updated monthly listeners for artist '" + artistToUpdate->getName() + "' to " + std::to_string(artistToUpdate->getMonListeners()) + ".");
            break;
        }
        case 5: {
            artistToUpdate->addSong();
            break;
        }
        case 6: {
            artistToUpdate->addRemasteredSong();
            break;
        }
        case 7: {
            artistToUpdate->updateSong();
            break;
        }
        case 8: {
            artistToUpdate->readAllSongs();
            break;
        }
        case 9: {
            artistToUpdate->readSong();
            break;
        }
        case 10: {
            addSongToPlaylist(artistToUpdate);
            break;
        }
        default: {
            std::cout << "[!] Invalid option, we will return you to the main menu." << std::endl;
        }
    }
}

void MusicPlatform::showMenu() {
    std::cout << "============================= " << name << " Menu =============================" << std::endl;
    std::cout << "1. Create a user" << std::endl;
    std::cout << "2. Create an artist" << std::endl;
    std::cout << "3. Delete a user" << std::endl;
    std::cout << "4. Show all users" << std::endl;
    std::cout << "5. Show details of a user" << std::endl;
    std::cout << "6. Show all artists" << std::endl;
    std::cout << "7. Show details of an artist" << std::endl;
    std::cout << "8. Modify a user" << std::endl;
    std::cout << "9. Modify an artist" << std::endl;
    std::cout << "10. Add a podcast" << std::endl;
    std::cout << "11. Show all podcasts" << std::endl;
    std::cout << "12. Add a podcast to a user's playlist" << std::endl;
    std::cout << "13. Show History & Undo last action" << std::endl;
    std::cout << "14. Show Global Artist Ranking" << std::endl;
    std::cout << "15. Show only PREMIUM users" << std::endl;
    std::cout << "==============================================================================" << std::endl;
    std::cout << std::endl << "Please choose one of the options from above (Enter 0 to exit): ";
    std::cin >> option;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(256, '\n');
        option = -1;
    }

    switch (option) {
        case 0: {
            std::cout << "[!] You closed the app!" << std::endl;
            break;
        }
        case 1: {
            addUser();
            option = -1;
            break;
        }
        case 2: {
            addArtist();
            option = -1;
            break;
        }
        case 3: {
            deleteUser();
            option = -1;
            break;
        }
        case 4: {
            showAllUsers();
            option = -1;
            break;
        }
        case 5: {
            showUser();
            option = -1;
            break;
        }
        case 6: {
            showAllArtists();
            option = -1;
            break;
        }
        case 7: {
            showArtist();
            option = -1;
            break;
        }
        case 8: {
            updateUser();
            option = -1;
            break;
        }
        case 9: {
            updateArtist();
            option = -1;
            break;
        }
        case 10: {
            addGlobalPodcast();
            option = -1;
            break;
        }
        case 11: {
            showAllPodcasts();
            option = -1;
            break;
        }
        case 12: {
            if (globalPodcasts.empty()) {
                std::cout << "[!] There are no podcasts created." << std::endl;
                return;
            }
            int tempIdPodcast;
            showAllPodcasts();

            Podcast *podcastToAdd = nullptr;
            while (true) {
                std::cout << "* Enter the ID of the podcast you want to add: ";
                std::cin >> tempIdPodcast;

                try {
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(256, '\n');
                        throw InvalidDataException("Invalid input! Please enter a numeric ID.",
                                                   static_cast<float>(tempIdPodcast));
                    }
                    podcastToAdd = getPodcast(tempIdPodcast);
                    if (!podcastToAdd) {
                        throw ItemNotFoundException("The ID you entered is invalid!", tempIdPodcast);
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
            if (users.empty()) {
                std::cout << "[!] There are no users created." << std::endl;
                return;
            }
            showAllUsers();
            int tempIdUser;

            User *tempUser = nullptr;
            while (true) {
                std::cout << "* Enter the ID of the User: ";
                std::cin >> tempIdUser;

                try {
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(256, '\n');
                        throw InvalidDataException("Invalid input! Please enter a numeric ID.",
                                                   static_cast<float>(tempIdUser));
                    }
                    tempUser = getUser(tempIdUser);
                    if (!tempUser) {
                        throw ItemNotFoundException("The ID you entered is invalid!", tempIdUser);
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

            if (tempUser->getnumberPlaylists() == 0) {
                std::cout << "[!] This user has no playlists created." << std::endl;
                return;
            }
            tempUser->showAllPlaylists();
            int tempIndex;

            Playlist *tempPlaylist = nullptr;
            while (true) {
                std::cout << "* Enter the index of the playlist: ";
                std::cin >> tempIndex;

                try {
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(256, '\n');
                        throw InvalidDataException("Invalid input! Please enter a numeric index.",
                                                   static_cast<float>(tempIndex));
                    }
                    tempPlaylist = (*tempUser)[tempIndex];
                    if (!tempPlaylist) {
                        throw ItemNotFoundException("The ID you entered is invalid!", tempIndex);
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
            try {
                tempPlaylist->addItem(podcastToAdd);
                std::cout << "[+] '" << podcastToAdd->getTitle() << "' added to playlist '" << tempPlaylist->getPlaylistName() << "'." << std::endl;
            } catch (const DuplicateItemException &e) {
                std::cout << "[!] DUPLICATE ERROR: " << e.what() << std::endl;
            } catch (const InvalidDataException &e) {
                std::cout << "[!] ERROR: " << e.what() << std::endl;
            } catch (const Exceptions &e) {
                std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
            }
            break;
        }
        case 13: {
            std::cout << std::endl << "==== RECENT ACTIONS ====" << std::endl;
            actionHistory.printLog();
            std::cout << "========================" << std::endl;

            std::vector<std::string> backupVector;
            actionHistory.exportToExternal(backupVector);
            std::ofstream fout("backup_history.txt");
            if (fout.is_open()) {
                for (const std::string& action : backupVector) {
                    fout << action << std::endl;
                }
                fout.close();
                std::cout << "[i] System: Successfully backed up " << backupVector.size() << " actions." << std::endl;
            } else {
                std::cout << "[!] System: Failed to open backup file!" << std::endl;
            }

            try {
                std::string last = actionHistory.undoAction();
                std::cout << "[!] UNDO SUCCESS: Removed action -> " << last << std::endl;
            } catch (const std::underflow_error& e) {
                std::cout << "[!] ERROR: " << e.what() << std::endl;
            }
            option = -1;
            break;
        }
        case 14: {
            showGlobalRanking();
            option = -1;
            break;
        }
        case 15: {
            if (users.empty()) {
                std::cout << "[!] There are no users created." << std::endl;
            } else if (premiumUsers == 0) {
                std::cout << "[!] There are no Premium users created." << std::endl;
            } else {
                std::cout << std::endl << "=== PREMIUM USERS ====" << std::endl;
                int index = 1;

                std::for_each(users.begin(), users.end(), [&index](const auto& element) {
                    const auto& [id, userPtr] = element;
                    if (userPtr->getsubscription() == 'P') {
                        std::cout << index++ << ". " << userPtr->getuserName() << " (ID: " << id << ")" << std::endl;
                    }
                });

                std::cout << "----------------------" << std::endl;
                std::cout << "Total Premium Users: " << premiumUsers << std::endl;

                std::streamsize prevPrec = std::cout.precision();
                std::ios_base::fmtflags prevFlags = std::cout.flags();

                std::cout << "Monthly Subscription Price: $" << std::fixed << std::setprecision(2) << User::getSubscriptionPrice() << std::endl;
                std::cout << "Estimated Monthly Revenue: $" << (premiumUsers * User::getSubscriptionPrice()) << std::endl;

                std::cout.flags(prevFlags);
                std::cout.precision(prevPrec);
            }
            option = -1;
            break;
        }
        default: {
            std::cout << "[!] Invalid option, please choose one of the options from above." << std::endl;
        }
    }
}

void MusicPlatform::seedDemoData() {
    Artist* weeknd = new Artist("The Weeknd", 115000000, 2010);
    Song* s1 = new Song("Blinding Lights", "The Weeknd", 3.20f, 9.8f, 29, 11, 2019);
    Song* s2 = new Remastered("Starboy", "The Weeknd", 3.50f, 9.5f, "Republic Studios", 2023, 21, 9, 2016);
    Song* s3 = new Song("After Hours", "The Weeknd", 6.01f, 9.7f, 19, 2, 2020);
    weeknd->addExistingSong(s1);
    weeknd->addExistingSong(s2);
    weeknd->addExistingSong(s3);
    artists[weeknd->getId()] = weeknd;

    Artist* tameImpala = new Artist("Tame Impala", 28500000, 2007);
    Song* s4 = new Song("The Less I Know The Better", "Tame Impala", 3.36f, 9.6f, 17, 7, 2015);
    Song* s5 = new Song("Let It Happen", "Tame Impala", 7.47f, 9.9f, 10, 3, 2015);
    Song* s6 = new Song("Borderline", "Tame Impala", 3.57f, 9.2f, 12, 4, 2019);
    tameImpala->addExistingSong(s4);
    tameImpala->addExistingSong(s5);
    tameImpala->addExistingSong(s6);
    artists[tameImpala->getId()] = tameImpala;

    Artist* rhcp = new Artist("Red Hot Chili Peppers", 34002300, 1983);
    Song* s7 = new Remastered("Californication", "Red Hot Chili Peppers", 5.29f, 9.7f, "Ocean Way Recording", 2014, 8, 6, 1999);
    Song* s8 = new Song("Snow (Hey Oh)", "Red Hot Chili Peppers", 5.34f, 9.5f, 9, 5, 2006);
    Song* s9 = new Remastered("Under the Bridge", "Red Hot Chili Peppers", 4.24f, 9.8f, "Warner Bros Studios", 2011, 10, 3, 1992);
    rhcp->addExistingSong(s7);
    rhcp->addExistingSong(s8);
    rhcp->addExistingSong(s9);
    artists[rhcp->getId()] = rhcp;

    Artist* daftPunk = new Artist("Daft Punk", 22376000, 1993);
    Song* s10 = new Song("Instant Crush", "Daft Punk", 5.37f, 9.4f, 17, 5, 2013);
    Song* s11 = new Remastered("One More Time", "Daft Punk", 5.20f, 9.6f, "Daft House Paris", 2021, 13, 11, 2000);
    daftPunk->addExistingSong(s10);
    daftPunk->addExistingSong(s11);
    artists[daftPunk->getId()] = daftPunk;

    Artist* arcticMonkeys = new Artist("Arctic Monkeys", 51450000, 2002);
    Song* s12 = new Song("Do I Wanna Know?", "Arctic Monkeys", 4.32f, 9.6f, 19, 6, 2013);
    Song* s13 = new Song("505", "Arctic Monkeys", 4.13f, 9.5f, 23, 4, 2007);
    arcticMonkeys->addExistingSong(s12);
    arcticMonkeys->addExistingSong(s13);
    artists[arcticMonkeys->getId()] = arcticMonkeys;

    Artist* pinkFloyd = new Artist("Pink Floyd", 19588000, 1965);
    Song* s14 = new Remastered("Comfortably Numb", "Pink Floyd", 6.22f, 10.0f, "Abbey Road Studios", 2011, 30, 11, 1979);
    pinkFloyd->addExistingSong(s14);
    artists[pinkFloyd->getId()] = pinkFloyd;

    Podcast* p1 = new Podcast("The Daily Tech Brief", 25.45f, "Alex Rivera");
    Podcast* p2 = new Podcast("Tape Notes: Music Production", 58.30f, "John Kennedy");
    Podcast* p3 = new Podcast("Core C++ & Systems Architecture", 44.15f, "Herb Sutter");
    Podcast* p4 = new Podcast("Song Exploder", 19.50f, "Hrishikesh Hirway");
    globalPodcasts.push_back(p1);
    globalPodcasts.push_back(p2);
    globalPodcasts.push_back(p3);
    globalPodcasts.push_back(p4);

    User* u1 = new User("alex", 'P');
    premiumUsers++;
    u1->recordMinutes(95);
    u1->recordMinutes(140);
    u1->recordMinutes(45);
    u1->recordMinutes(110);

    Playlist* pl1 = new Playlist("Late Night Coding", 'N');
    pl1->addItem(s3);
    pl1->addItem(s5);
    pl1->addItem(s10);
    pl1->addItem(p3);
    u1->addExistingPlaylist(pl1);

    Playlist* pl2 = new Playlist("Synth & Electronic", 'P');
    pl2->addItem(s1);
    pl2->addItem(s2);
    pl2->addItem(s11);
    u1->addExistingPlaylist(pl2);
    users[u1->getId()] = u1;

    User* u2 = new User("elena_guitars", 'F');
    u2->recordMinutes(65);
    u2->recordMinutes(30);
    u2->recordMinutes(75);

    Playlist* pl3 = new Playlist("Guitar Riffs & Classics", 'N');
    pl3->addItem(s7);
    pl3->addItem(s8);
    pl3->addItem(s9);
    pl3->addItem(s12);
    pl3->addItem(s14);
    u2->addExistingPlaylist(pl3);
    users[u2->getId()] = u2;

    User* u3 = new User("marcus_studio", 'P');
    premiumUsers++;
    u3->recordMinutes(180);
    u3->recordMinutes(210);

    Playlist* pl4 = new Playlist("Studio Reference Tracks", 'N');
    pl4->addItem(s4);
    pl4->addItem(s6);
    pl4->addItem(s13);
    pl4->addItem(p2);
    pl4->addItem(p4);
    u3->addExistingPlaylist(pl4);
    users[u3->getId()] = u3;

    User* u4 = new User("diana", 'F');
    u4->recordMinutes(25);
    u4->recordMinutes(40);

    Playlist* pl5 = new Playlist("Morning Commute", 'P');
    pl5->addItem(p1);
    pl5->addItem(s1);
    pl5->addItem(s12);
    u4->addExistingPlaylist(pl5);
    users[u4->getId()] = u4;

    actionHistory.addAction("System initialized with default catalog");
    Logger::getInstance().logEvent("Demo catalog seeded successfully.");
}