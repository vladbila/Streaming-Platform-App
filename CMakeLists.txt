cmake_minimum_required(VERSION 4.1)
project(Streaming_Platform_App)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(Streaming_Platform_App
        main.cpp
        include/Logger.h
        include/Exceptions.h
        include/MediaItem.h
        include/Artist.h
        include/Playlist.h
        include/User.h
        include/HistoryLog.h
        include/MusicPlatform.h
        src/Artist.cpp
        src/Playlist.cpp
        src/User.cpp
        src/MusicPlatform.cpp
        include/IObject.h
        include/ReleaseDate.h
        src/ReleaseDate.cpp
        include/Song.h
        src/Song.cpp
        include/Remastered.h
        src/Remastered.cpp
        include/Podcast.h
        src/Podcast.cpp)
