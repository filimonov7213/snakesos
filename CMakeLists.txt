cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)

# Aggiungi tutte le cartelle sorgente
include_directories(${CMAKE_SOURCE_DIR}/libraries)

file(GLOB SOURCES
        ${CMAKE_SOURCE_DIR}/libraries/*.cpp
        ${CMAKE_SOURCE_DIR}/source/*.cpp
)

add_executable(SnakeGame ${SOURCES})

# Link librerie diverse a seconda del sistema operativo
if (WIN32)
    target_link_libraries(SnakeGame pdcurses)
elseif(UNIX)
    target_link_libraries(SnakeGame ncurses)
endif()