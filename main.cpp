#include <iostream>
#include <cctype>
#include "MusicPlatform.h"

int main() {
    MusicPlatform spotify("Streaming Platform");

    char loadCatalog;
    std::cout << "* Load default music catalog and sample users? (y/n): ";
    std::cin >> loadCatalog;
    if (std::tolower(loadCatalog) == 'y') {
        spotify.seedDemoData();
        std::cout << "[+] Default catalog loaded successfully!" << std::endl << std::endl;
    }

    do {
        spotify.showMenu();
    } while (spotify.getOption() != 0);

    return 0;
}