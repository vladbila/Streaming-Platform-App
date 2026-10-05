#include "Remastered.h"
#include "Exceptions.h"
#include <iostream>
#include <sstream>

Remastered::Remastered(const std::string& t, const std::string& a, float d, float r, const std::string& s, int ry, int day, int month, int year)
    : Song(t, a, d, r, day, month, year), studio(s), year(ry) {}

void Remastered::play() const {
    std::cout << "[>] Now playing (High-Res Remastered Audio): '" << title << "' by " << artist << std::endl;
}

void Remastered::readDetails() {
    Song::readDetails();
    while (true) {
        std::cout << "* Enter the remastering year: ";
        std::cin >> year;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(256, '\n');
                throw InvalidDataException("Please enter a valid number, not text!", static_cast<float>(year));
            }

            if (year < 1900 || year > 2026) {
                throw InvalidDataException("The remastering year must be between 1900 and 2026.",
                                           static_cast<float>(year));
            }
            break;
        } catch (const InvalidDataException &e) {
            std::cout << "[!] ERROR: " << e.what() << std::endl;
            std::cout << "[!] Please try again." << std::endl << std::endl;
        } catch (const Exceptions &e) {
            std::cout << "[!] PLATFORM ERROR: " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[!] UNKNOWN ERROR" << std::endl;
        }
    }
    std::cin.get();

    std::cout << "* Enter the remastering studio: ";
    std::getline(std::cin, studio);
}

std::string Remastered::toString() const {
    std::ostringstream os;
    os << Song::toString() << std::endl;
    os << "   [!] REMASTERED EDITION" << std::endl;
    os << "   * Remastering year: " << year << std::endl;
    os << "   * Studio: " << studio << std::endl;
    os << "=================================" << std::endl;

    return os.str();
}