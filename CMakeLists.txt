cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Includi le librerie personalizzate
include_directories(libraries)

# File sorgenti (sources + libraries)
file(GLOB_RECURSE SOURCES
        "sources/*.cpp"
        "libraries/*.cpp"
)

# Crea eseguibile
add_executable(SnakeGame ${SOURCES})

# --- Gestione libreria curses cross-platform ---
if (WIN32)
    # Su Windows usiamo PDCurses
    # Supponiamo che tu abbia PDCurses in C:/pdcurses
    include_directories("C:/pdcurses")
    link_directories("C:/pdcurses")
    target_link_libraries(SnakeGame pdcurses)
else()
    # Su Linux/macOS usiamo ncurses
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    target_link_libraries(SnakeGame ${CURSES_LIBRARIES})
endif()
