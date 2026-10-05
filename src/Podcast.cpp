#include "Podcast.h"
#include "Exceptions.h"
#include <iostream>
#include <sstream>
#include <cmath>

Podcast::Podcast(const std::string& t, float d, const std::string& h) : MediaItem(t, d), host(h) {}

void Podcast::play() const {
    std::cout << "[>] Now playing Podcast '" << title << "' (Hosted by " << host << ")" << std::endl;
}

void Podcast::readDetails() {
    std::cout << "* Enter the name of the Podcast: ";
    std::cin.get();
    std::getline(std::cin, title);

    std::cout << "* Enter the host name: ";
    std::getline(std::cin, host);

    while (true) {
        std::cout << "* Enter the duration of the podcast (e.g. 45.30): ";
        std::cin >> duration;
        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Please enter a valid number, not text!", duration);
            }
            if (duration < 0.0)
                throw InvalidDataException("Duration must be greater than 0.", duration);
            int minutes = static_cast<int>(duration);
            int seconds = static_cast<int>(std::round((duration - static_cast<float>(minutes)) * 100));
            if (seconds > 59)
                throw InvalidDataException("The seconds part cannot be 60 or greater! (e.g. use 4.00 instead of 3.60)",
                                           duration);
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
    std::cin.ignore();
}

std::string Podcast::toString() const {
    std::ostringstream os;
    os << "======== Podcast ID : " << getId() << " =========" << std::endl;
    os << "* Title: " << title << std::endl;
    os << "* Host: " << host << std::endl;
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
    os << "===================================" << std::endl << std::endl;

    return os.str();
}