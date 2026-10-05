#include "ReleaseDate.h"
#include "Exceptions.h"
#include <iostream>

ReleaseDate::ReleaseDate(int d, int m, int y) : day(d), month(m), year(y) {}

void ReleaseDate::readDate() {
    while (true) {
        std::cout << "* Enter the release date of the song (day month year): ";
        std::cin >> day >> month >> year;
        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Invalid release date format! (non-numeric format)", -1);
            }
            if (day < 1 || day > 31)
                throw InvalidDataException("Day is out of valid bounds!", static_cast<float>(day));
            if (month < 1 || month > 12)
                throw InvalidDataException("Month is out of valid bounds!", static_cast<float>(month));
            if (year < 1800 || year > 2026)
                throw InvalidDataException("Year is out of valid bounds!", static_cast<float>(year));
            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "[!] Please enter the release date of the song again (day month year)!" << std::endl << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR OCCURRED!" << std::endl;
        }
    }
}

std::string ReleaseDate::toString() const {
    return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
}