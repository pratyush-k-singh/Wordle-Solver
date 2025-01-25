cmake_minimum_required(VERSION 3.10)
project(wordle_solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}  # For MOC files
)

# Add library for core solver
add_library(solver_lib
    src/wordle_solver.cpp
)

# Set the sources for the main executable
set(WORDLE_SOLVER_SOURCES
    src/main.cpp
    src/wordle_solver_window.cpp
    include/wordle_solver_window.hpp
)

# Add executable
add_executable(wordle_solver ${WORDLE_SOLVER_SOURCES})

# Ensure MOC files are generated and included
set_target_properties(wordle_solver PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Link libraries
target_link_libraries(wordle_solver PRIVATE
    solver_lib
    Qt6::Core
    Qt6::Widgets
)

# Optional: Build utilities only if dependencies are found
find_package(CURL)
find_package(Gumbo)

if(CURL_FOUND AND GUMBO_FOUND)
    add_executable(fetch_valid_words
        utils/fetch_valid_words.cpp
    )
    target_link_libraries(fetch_valid_words PRIVATE
        CURL::libcurl
        gumbo
    )
endif()

add_executable(initial_guesses
    utils/initial_guesses.cpp
)

# Copy data directory to build directory
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
