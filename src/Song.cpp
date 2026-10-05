#include "Song.h"
#include "Exceptions.h"
#include <iostream>
#include <sstream>
#include <cmath>

Song::Song() : MediaItem("Unknown", 0.0), artist("N/A"), rating(0.0) {}

Song::Song(const std::string& t, const std::string& a, float d, float r, int day, int m, int y)
    : MediaItem(t, d), artist(a), rating(r), launchDate(day, m, y) {}

void Song::play() const {
    std::cout << "[>] Now playing song: '" << title << "' by " << artist << std::endl;
}

void Song::setSongName(const std::string& newName) {
    if (newName.empty() || newName.size() < 2) {
        throw InvalidDataException("The name must contain at least 2 characters!", newName.size());
    }
    title = newName;
}

void Song::setArtistName(const std::string& newName) {
    if (newName.empty() || newName.size() < 2) {
        throw InvalidDataException("The name must contain at least 2 characters!", newName.size());
    }
    artist = newName;
}

void Song::setLength(const float length) {
    validateLength(length);
    duration = length;
}

void Song::setRating(const float newRating) {
    if (newRating < 0.0 || newRating > 10.0) {
        throw InvalidDataException("The rating must be between 0.0 and 10.0!", newRating);
    }
    rating = newRating;
}

void Song::adjustDurationMinutes(int deltaMinutes) {
    float newDuration = duration + static_cast<float>(deltaMinutes);
    duration = (newDuration < 0.0f) ? 0.0f : newDuration;
}

void Song::adjustRating(float delta) {
    setRating(rating + delta);
}

void Song::validateLength(const float length) {
    if (length < 0.0) {
        throw InvalidDataException("The length of the song can't be negative!", length);
    }
    int minutes = static_cast<int>(length);
    int seconds = static_cast<int>(std::round((length - static_cast<float>(minutes)) * 100));
    if (seconds > 59) {
        throw InvalidDataException("The seconds part cannot exceed 59.", length);
    }
}

std::string Song::toString() const {
    std::ostringstream os;
    os << "======== Song ID : " << getId() << " =========" << std::endl;
    os << "* Song Name: " << title << std::endl;
    os << "* Artist: " << artist << std::endl;
    os << "* Released on: " << launchDate.toString() << std::endl;
    if (duration == 0.0) {
        os << "* Length: N/A" << std::endl;
    } else {
        int minutes = static_cast<int>(duration);
        int seconds = std::round((duration - minutes) * 100);
        os << "* Length: " << minutes << ":";
        if (seconds < 10) {
            os << "0";
        }
        os << seconds << std::endl;
    }

    if (rating == 0.0)
        os << "* Song Rating: N/A" << std::endl;
    else
        os << "* Song Rating: " << rating << std::endl;
    os << "=================================" << std::endl;

    return os.str();
}

void Song::readDetails() {
    std::cout << "* Enter the name of the song: ";
    std::cin.get();
    std::getline(std::cin, title);

    while (true) {
        std::cout << "* Enter the length of the song (type: mm.ss): ";
        std::cin >> duration;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid characters! Please enter a number (e.g., 3.45).", duration);
            }
            validateLength(duration);
            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "[!] Please try again!" << std::endl << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
        }
    }

    while (true) {
        std::cout << "* Enter the rating of the song: ";
        std::cin >> rating;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Please enter a valid number, not text!", rating);
            }

            if (rating < 0.0 || rating > 10.0) {
                throw InvalidDataException("The rating must be between 0.0 and 10.0.", rating);
            }

            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "[!] Please try again." << std::endl << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR!" << std::endl;
        }
    }
    std::cin.ignore();

    launchDate.readDate();
}

bool Song::operator==(const Song &m) const {
    const float eps = 1e-5;
    return std::abs(rating - m.rating) <= eps;
}

bool Song::operator<(const Song &m) const {
    return rating < m.rating;
}

bool Song::operator<=(const Song &m) const {
    return this->rating < m.rating || *this == m;
}