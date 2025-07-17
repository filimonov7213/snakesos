cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Trova ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Includi le tue librerie personalizzate
include_directories(libraries)

# File sorgenti
file(GLOB_RECURSE SOURCES
        "sources/*.cpp"
        "libraries/*.cpp"
)

# Crea eseguibile
add_executable(SnakeGame ${SOURCES})

# Collega la libreria ncurses
target_link_libraries(SnakeGame ${CURSES_LIBRARIES})
