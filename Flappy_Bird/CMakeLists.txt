cmake_minimum_required(VERSION 3.27)
project(Flappy_Bird)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.0)
FETCHCONTENT_MAKEAVAILABLE(SFML)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES
        main.cpp
        Bird.cpp
        Pipe.cpp
        Engine.cpp
        Difficulty.h
        PlayerModel.h


)
add_executable(Flappy_Bird ${PROJECT_SOURCES}

)


target_link_libraries(Flappy_Bird sfml-graphics)
target_link_libraries(Flappy_Bird sfml-audio)
file(COPY res DESTINATION ${CMAKE_BINARY_DIR})