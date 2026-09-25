cmake_minimum_required(VERSION 3.28)

project(epollExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(epollExercises
    main.cpp
    EpollChatServer.cpp
    EpollServer.cpp
    PollServer.cpp
    SelectServer.cpp
)

target_compile_options(epollExercises
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)