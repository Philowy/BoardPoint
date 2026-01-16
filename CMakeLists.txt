cmake_minimum_required(VERSION 4.0)

project(BoardPoint)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

FetchContent_MakeAvailable(SFML)

add_executable(BoardPoint WIN32 main.cpp
        classes/Test.cpp
        classes/Test.hpp
        Sceneries/Menu.cpp
        Sceneries/Menu.hpp
        Enums.hpp)

target_link_libraries(BoardPoint PRIVATE SFML::Graphics)

target_link_options(${PROJECT_NAME} PRIVATE "-static-libgcc" "-static")

