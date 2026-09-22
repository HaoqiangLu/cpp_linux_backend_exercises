cmake_minimum_required(VERSION 3.28)

project(fileIO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(fileIO
    main.cpp
    ConfigParser.cpp
    FileIoExercises.cpp
)

target_compile_options(fileIO
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)