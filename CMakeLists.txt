cmake_minimum_required(VERSION 3.10)
project(wordle_solver)

set(CMAKE_CXX_STANDARD 17)

# Add the include directories
include_directories(include)

# Add source files from src and utils directories
set(SOURCES
    src/main.cpp
    src/wordle_solver.cpp
    utils/fetch_valid_words.cpp
    utils/initial_guesses.cpp
)

add_executable(wordle_solver ${SOURCES})
