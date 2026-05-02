cmake_minimum_required(VERSION 3.16)
project(Fireaim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/player.cpp
    src/target.cpp
    src/projectile.cpp
    src/renderer.cpp
)

# Header files
set(HEADERS
    include/game.h
    include/player.h
    include/target.h
    include/projectile.h
    include/renderer.h
    include/constants.h
)

# Create executable
add_executable(fireaim ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(fireaim PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    OpenGL::GL
)

# Include directories
target_include_directories(fireaim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

# Enable testing
enable_testing()

# Simple test executable
add_executable(test_game test/test_game.cpp src/game.cpp)
target_link_libraries(test_game PRIVATE SDL2::SDL2)