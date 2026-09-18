cmake_minimum_required(VERSION 3.28)
project(modernCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(modernCpp
    main.cpp
    ModernExercises.cpp
    ResourceManager.cpp
)

target_compile_options(modernCpp
    PRIVATE
    -Wall
    -Wextra
)
