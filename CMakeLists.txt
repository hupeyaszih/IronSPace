cmake_minimum_required(VERSION 3.16)

project(IronSPace LANGUAGES C)

set(CMAKE_C\_STANDARD 11)
set(CMAKE_C\_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find the SDL2 package
find_package(SDL2 REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(IronSPace
    src/main.c
    src/renderer.c
    src/player.c
    src/game.c
)

target_link_libraries(IronSPace ${SDL2_LIBRARIES} m)

target_include_directories(IronSPace
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
