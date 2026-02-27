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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(BoardPoint WIN32 main.cpp
#add_executable(BoardPoint main.cpp
        Sceneries/MenuScene.hpp
        Sceneries/Scene.hpp
        Sceneries/MenuScene.cpp
        Sceneries/AppScene.cpp
        Sceneries/AppScene.hpp
        Classes/ClassText.cpp
        Classes/ClassText.hpp
        Classes/LeftPanel/LeftPanel.cpp
        Classes/LeftPanel/LeftPanel.hpp
        Classes/LeftPanel/ListItem.cpp
        Classes/LeftPanel/ListItem.hpp
        windowSizeClass.hpp)

target_link_libraries(BoardPoint PRIVATE SFML::Graphics)

target_link_options(${PROJECT_NAME} PRIVATE "-static-libgcc" "-static")

