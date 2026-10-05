#  Music & Podcast Streaming Platform — Admin & User Simulator (C++)

A modular, console-based audio streaming management system and simulator built in Modern C++ (C++17/C++20). Unlike a standard client-only player, this application operates as a dual **Platform Administrator Console & Interactive User Environment**—allowing you to manage the entire backend ecosystem (users, artists, discographies, podcasts, and revenue analytics) while simultaneously simulating end-user interactions (playlist curation, audio playback, and listening statistics).

---

##  Core Capabilities (Admin + User Hybrid)

* **Platform Administration & CRUD Operations:**
    * **User Management:** Create and delete user accounts, toggle subscription tiers (Free vs. Premium), filter Premium subscribers, and calculate estimated monthly platform revenue.
    * **Artist & Discography Control:** Create artist profiles, adjust monthly listener counts, view the Global Artist Ranking, and release or modify `Song` and `Remastered` tracks (updating titles, durations, and ratings).
    * **Global Podcast Catalog:** Publish and sort global podcast episodes by duration.
    * **System Auditing & Rollback:** Track real-time platform events in `system_log.txt`, export action history backups to `backup_history.txt`, and undo recent administrative/user actions.
* **End-User Experience:**
    * **Playlist Curation:** Create public or private playlists, add songs and podcasts from the global catalog, remove items, and automatically sort tracks by rating.
    * **Media Playback:** Simulate polymorphic audio playback for standard songs, high-res remastered editions, and podcasts.
    * **Listening Analytics:** Log daily listening minutes and compute personal user statistics (total time, single-day record, and high-activity days).

---

##  C++ Architecture & Technical Highlights

* **Object-Oriented Hierarchy & Polymorphism:**
    * `IObject`: Abstract root interface providing automatic, unique ID generation (`inline static` counter) and polymorphic stream insertion (`operator<<` via virtual `toString()`).
    * `MediaItem`: Abstract base class for playable audio entities (`play()`, `readDetails()`).
    * `Song`, `Remastered`, and `Podcast`: Derived classes utilizing runtime polymorphism, upcasting, and safe downcasting (`dynamic_cast`) for sorting mixed-media playlists and deep-copying discographies.
* **Memory Management (Rule of Three):**
    * Custom copy constructors, copy assignment operators (`operator=`), and virtual destructors across `Artist` and `User` to manage dynamically allocated `Song*` and `Playlist*` collections safely without memory leaks.
* **Custom Exception Hierarchy:**
    * Base `Exceptions` class inheriting from `std::exception`, specialized into `InvalidDataException`, `ItemNotFoundException`, and `DuplicateItemException` for input validation and lookup safety.
* **Templates & Generic Programming:**
    * `HistoryLog<T, MaxCapacity>`: Bounded template container supporting undo operations (`undoAction()`), capacity checks, and generic container export (`exportToExternal<U>()`).
    * Function template `notifyPlatform<T>()` with explicit specialization for `std::string`.
* **Design Patterns:**
    * **Singleton Pattern:** Thread-safe Meyer's Singleton `Logger` class that records timestamped platform and user events to `system_log.txt`.
* **STL Containers & Algorithms:**
    * Extensive use of `std::map`, `std::vector`, and `std::list` alongside `<algorithm>` functions (`std::sort`, `std::find_if`, `std::count_if`, `std::for_each`, `std::remove_if`) and C++ lambda expressions.

---

##  Project Structure

```text
Streaming-Platform-App/
├── include/               # Header files (.h)
│   ├── IObject.h
│   ├── Exceptions.h
│   ├── Logger.h
│   ├── HistoryLog.h
│   ├── MediaItem.h
│   ├── ReleaseDate.h
│   ├── Song.h
│   ├── Remastered.h
│   ├── Podcast.h
│   ├── Artist.h
│   ├── Playlist.h
│   ├── User.h
│   └── MusicPlatform.h
├── src/                   # Implementation files (.cpp)
│   ├── ReleaseDate.cpp
│   ├── Song.cpp
│   ├── Remastered.cpp
│   ├── Podcast.cpp
│   ├── Artist.cpp
│   ├── Playlist.cpp
│   ├── User.cpp
│   └── MusicPlatform.cpp
├── main.cpp               # Application entry point
└── CMakeLists.txt         # CMake build configuration
```

---

##  Getting Started

### Prerequisites
* A C++ compiler supporting C++17 / C++20 (GCC, Clang, or MSVC)
* CMake (version 3.20 or higher)

### Build & Run
```bash
# Clone the repository
git clone [https://github.com/vladbila/Streaming-Platform-App.git](https://github.com/vladbila/Streaming-Platform-App.git)
cd Streaming-Platform-App

# Configure and build with CMake
cmake -B build
cmake --build build

# Run the application
./build/Streaming_Platform_App
```
*(Tip: Upon startup, enter `y` when prompted to automatically seed the platform with sample artists, songs, podcasts, and user playlists).*