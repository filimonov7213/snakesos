cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(libraries)

file(GLOB_RECURSE SOURCES
        "source/*.cpp"
        "libraries/*.cpp"
)

add_executable(SnakeGame ${SOURCES}
        libraries/leaderboard.cpp
        libraries/leaderboard.h)

if (WIN32)
    # Path PDCurses (aggiusta se serve)
    include_directories("C:/pdcurses")
    link_directories("C:/pdcurses")

    target_link_libraries(SnakeGame pdcurses)

    # 👇 questa riga è fondamentale
    set_target_properties(SnakeGame PROPERTIES LINK_FLAGS "-mconsole")
else()
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    target_link_libraries(SnakeGame ${CURSES_LIBRARIES})
endif()
