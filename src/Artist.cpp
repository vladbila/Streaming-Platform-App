#include "Artist.h"
#include "Remastered.h"
#include "Exceptions.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Artist::Artist() : name("N/A"), monListeners(0), DebutYear(0) {}

Artist::Artist(const std::string& name, int monListeners, int DebutYear)
    : name(name), monListeners(monListeners), DebutYear(DebutYear) {}

Artist::Artist(const Artist &other)
    : name(other.name), monListeners(other.monListeners), DebutYear(other.DebutYear) {
    for (Song* song : other.discography) {
        if (auto* rem = dynamic_cast<Remastered*>(song)) {
            discography.push_back(new Remastered(*rem));
        } else {
            discography.push_back(new Song(*song));
        }
    }
}

Artist::~Artist() noexcept {
    for (Song* song : discography) {
        delete song;
    }
    discography.clear();
}

Artist &Artist::operator=(const Artist &other) {
    if (this == &other)
        return *this;
    monListeners = other.monListeners;
    DebutYear = other.DebutYear;
    name = other.name;
    for (Song* song : discography) {
        delete song;
    }
    discography.clear();
    for (Song* song : other.discography) {
        if (auto* rem = dynamic_cast<Remastered*>(song)) {
            discography.push_back(new Remastered(*rem));
        } else {
            discography.push_back(new Song(*song));
        }
    }
    return *this;
}

void Artist::setName(const std::string& newName) {
    if (newName.empty() || newName.size() < 2)
        throw InvalidDataException("The name must contain at least 2 characters!", newName.size());
    name = newName;
}

void Artist::setMonListeners(const int newMonListeners) {
    if (newMonListeners < 0)
        throw InvalidDataException("The artist's monthly listeners can't be negative",
                                   static_cast<float>(newMonListeners));
    monListeners = newMonListeners;
}

void Artist::adjustMonthlyListeners(int delta) {
    monListeners += delta;
    if (monListeners < 0) {
        monListeners = 0;
    }
}

bool Artist::operator==(const Artist &other) const {
    return monListeners == other.monListeners;
}

bool Artist::operator<(const Artist &other) const {
    return monListeners < other.monListeners;
}

bool Artist::operator<=(const Artist &other) const {
    return *this < other || *this == other;
}

bool Artist::operator>(const Artist &other) const {
    return other < *this;
}

bool Artist::operator>=(const Artist &other) const {
    return !(*this < other);
}

Song *Artist::operator[](const int wantedId) const {
    for (Song* song : discography) {
        if (song->getId() == wantedId)
            return song;
    }
    return nullptr;
}

void Artist::addSong() {
    Song *newSong = new Song;
    newSong->readDetails();
    try {
        newSong->setArtistName(getName());
    } catch (const InvalidDataException &e) {
        std::cout << "[!] ERROR: " << e.what() << std::endl;
    } catch (const Exceptions& e) {
        std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
    }
    discography.push_back(newSong);
    Logger::getInstance().logEvent("Artist " + name + " added a new song: " + newSong->getTitle());

    std::cout << "[+] Song '" << newSong->getTitle() << "' has been added." << std::endl;
}

void Artist::addRemasteredSong() {
    Remastered *newSong = new Remastered;
    newSong->readDetails();
    try {
        newSong->setArtistName(getName());
    } catch (const InvalidDataException &e) {
        std::cout << "[!] ERROR: " << e.what() << std::endl;
    } catch (const Exceptions& e) {
        std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
    }
    discography.push_back(newSong);
    Logger::getInstance().logEvent("Artist " + name + " added a new REMASTERED song: " + newSong->getTitle());

    std::cout << "[+] Song '" << newSong->getTitle() << "' has been added." << std::endl;
}

void Artist::addExistingSong(Song* song) {
    if (song) {
        discography.push_back(song);
    }
}

void Artist::readAllSongs() const {
    if (discography.empty()) {
        std::cout << "[!] This artist has no songs!" << std::endl;
        return;
    }
    std::cout << "================== List of Songs ==================" << std::endl;
    for (int i = 0; i < discography.size(); i++) {
        std::cout << "# " << discography[i]->getTitle() << " by " << name << " (ID: " << discography[i]->getId() << ")" << std::endl;
    }
    std::cout << "===================================================" << std::endl;
}

void Artist::readSong() const {
    if (discography.empty()) {
        std::cout << "[!] This artist has no songs!" << std::endl;
        return;
    }
    int tempId;
    readAllSongs();

    Song *songToBeSearched = nullptr;
    while (true) {
        std::cout << "* Enter the ID of the song you want to search: ";
        std::cin >> tempId;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a valid ID!", static_cast<float>(tempId));
            }
            songToBeSearched = (*this)[tempId];
            if (!songToBeSearched) {
                throw ItemNotFoundException("This song couldn't be found!", tempId);
            }
            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "[!] Please try again." << std::endl;
        } catch (const ItemNotFoundException &e) {
            std::cout << "[!] SEARCH ERROR: " << e.what() << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
        }
    }
    std::cout << *songToBeSearched << std::endl;
}

void Artist::updateSong() {
    if (discography.empty()) {
        std::cout << "[!] This artist has no songs released." << std::endl;
        return;
    }
    int tempId;
    readAllSongs();

    Song *songToBeModified = nullptr;
    while (true) {
        std::cout << "* Enter the ID of the song you want to search: ";
        std::cin >> tempId;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid input! Please enter a valid ID!", static_cast<float>(tempId));
            }
            songToBeModified = (*this)[tempId];
            if (!songToBeModified) {
                throw ItemNotFoundException("This song couldn't be found!", tempId);
            }
            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "[!] Please try again." << std::endl;
        } catch (const ItemNotFoundException &e) {
            std::cout << "[!] SEARCH ERROR: " << e.what() << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
        }
    }

    int tempOption;
    std::cout << "================= Options =================" << std::endl;
    std::cout << "SONG : " << songToBeModified->getTitle() << std::endl;
    std::cout << "1. Change the name of the song." << std::endl;
    std::cout << "2. Change the duration of the song." << std::endl;
    std::cout << "3. Add a minute to the duration of the song." << std::endl;
    std::cout << "4. Subtract a minute from the duration of the song." << std::endl;
    std::cout << "5. Add a value to the rating." << std::endl;
    std::cout << "6. Subtract a value from the rating." << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "Choose an option(1-6): ";
    std::cin >> tempOption;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(256, '\n');
        tempOption = -1;
    }

    switch (tempOption) {
        case 1: {
            std::string oldName = songToBeModified->getTitle();
            std::string newName;
            std::cout << "* Enter the new name of the song: ";
            std::cin.get();
            std::getline(std::cin, newName);
            try {
                songToBeModified->setSongName(newName);
                std::cout << "[!] The name of the song has been changed." << std::endl;
                Logger::getInstance().logEvent("Song '" + oldName + "' renamed to '" + newName + "'.");
            } catch (const InvalidDataException &e) {
                std::cout << "[!] ERROR: " << e.what() << std::endl;
            } catch (const Exceptions &e) {
                std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
            } catch (...) {
                std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
            }
            break;
        }
        case 2: {
            float newDuration;

            while (true) {
                std::cout << "* Enter the new duration of the song: ";
                std::cin >> newDuration;
                try {
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(256, '\n');
                        throw InvalidDataException("Invalid input! Please enter a number!", newDuration);
                    }
                    songToBeModified->setLength(newDuration);
                    Logger::getInstance().logEvent("The duration of the song '" + songToBeModified->getTitle() + "' has been modified to " + std::to_string(newDuration) + ".");
                    std::cout << "[!] The length of the song has been changed." << std::endl;
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
            songToBeModified->adjustDurationMinutes(1);
            Logger::getInstance().logEvent("The duration of the song '" + songToBeModified->getTitle() + "' has been modified to " + std::to_string(songToBeModified->getDuration()) + ".");
            break;
        }
        case 4: {
            songToBeModified->adjustDurationMinutes(-1);
            Logger::getInstance().logEvent("The duration of the song '" + songToBeModified->getTitle() + "' has been modified to " + std::to_string(songToBeModified->getDuration()) + ".");
            break;
        }
        case 5: {
            float tempValue;

            while (true) {
                std::cout << "* Enter the value you want to add to the current rating(" << songToBeModified->getRating()
                        << "): ";
                std::cin >> tempValue;

                try {
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(256, '\n');
                        throw InvalidDataException("Invalid input! Please enter a number.", -1);
                    }

                    songToBeModified->adjustRating(tempValue);
                    Logger::getInstance().logEvent("The rating of the song '" + songToBeModified->getTitle() + "' has been modified to " + std::to_string(songToBeModified->getRating()) + ".");

                    std::cout << "[!] I modified the rating of the song " << songToBeModified->getTitle() << "." << std::endl;
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
        case 6: {
            float tempValue;
            while (true) {
                std::cout << "* Enter the value you want to subtract from the rating: ";
                std::cin >> tempValue;

                try {
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(256, '\n');
                        throw InvalidDataException("Invalid input! Please enter a number!", tempValue);
                    }

                    songToBeModified->adjustRating(-tempValue);
                    Logger::getInstance().logEvent("The rating of the song '" + songToBeModified->getTitle() + "' has been modified to " + std::to_string(songToBeModified->getRating()) + ".");

                    std::cout << "[!] I modified the rating of the song!" << std::endl;
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
        default: {
            std::cout << "[!] Invalid option, we will return you to the main menu." << std::endl;
        }
    }
}

std::istream &operator>>(std::istream &is, Artist &a) {
    is.get();
    while (true) {
        std::cout << "* Enter the name of the artist: ";
        std::getline(is, a.name);

        try {
            if (a.name.empty() || a.name.size() < 2) {
                throw InvalidDataException("The name of the artist must have more than 2 characters!", a.name.size());
            }
            break;
        } catch (const InvalidDataException& e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "[!] Please try again." << std::endl << std::endl;
        } catch (const Exceptions& e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
        }
    }

    while (true) {
        std::cout << "* Enter the artist's monthly listeners: ";
        is >> a.monListeners;
        try {
            if (is.fail()) {
                is.clear();
                is.ignore(256, '\n');
                throw InvalidDataException("Please enter a valid number, not text!", -1);
            }
            if (a.monListeners < 0)
                throw InvalidDataException("The number of monthly listeners can't be negative!", a.monListeners);
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

    while (true) {
        std::cout << "* Enter the debut year of the artist: ";
        is >> a.DebutYear;

        try {
            if (is.fail()) {
                is.clear();
                is.ignore(256, '\n');
                throw InvalidDataException("Please enter a valid number, not text!", -1);
            }
            if (a.getDebutYear() < 1800 || a.DebutYear > 2026) {
                throw InvalidDataException("The debut year introduced is invalid", static_cast<float>(a.DebutYear));
            }
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
    return is;
}

std::string Artist::toString() const {
    std::ostringstream os;

    os << "============= Artist ID : " << getId() << "=============" << std::endl;
    os << "* Artist name: " << name << std::endl;
    os << "* Monthly listeners: ";
    if (monListeners <= 0) {
        os << "N/A";
    } else if (monListeners < 10000)
        os << monListeners;
    else if (monListeners < 1000000) {
        std::streamsize prec = os.precision();
        std::ios_base::fmtflags flags = os.flags();
        auto listeners = static_cast<float>(monListeners) / 1000;
        os << std::fixed << std::setprecision(1) << listeners << " K";
        os.flags(flags);
        os.precision(prec);
    } else {
        std::streamsize prec = os.precision();
        std::ios_base::fmtflags flags = os.flags();
        auto listeners = static_cast<float>(monListeners) / 1000000;
        os << std::fixed << std::setprecision(1) << listeners << " M";
        os.flags(flags);
        os.precision(prec);
    }
    os << std::endl;
    os << "* Debut Year: ";
    if (DebutYear < 1800 || DebutYear > 2026) {
        os << "N/A";
    } else os << DebutYear << std::endl;
    os << "============================================" << std::endl;
    return os.str();
}