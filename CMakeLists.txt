cmake_minimum_required(VERSION 3.16)

project(IronSPace LANGUAGES C)

set(CMAKE_C\_STANDARD 11)
set(CMAKE_C\_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Copy assets folder to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Find the SDL2 package
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(IronSPace
    src/main.c
    src/renderer.c
    src/player.c
    src/game.c
    src/game_map.c
    src/asset_manager.c
)

target_link_libraries(IronSPace 
    PRIVATE 
        SDL2::SDL2 
        SDL2_image::SDL2_image
        m
)

target_include_directories(IronSPace
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
