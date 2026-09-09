cmake_minimum_required(VERSION 3.28)
project(setExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(setExercises
    main.cpp
    SetExercises.cpp
    Blacklist.cpp
)

target_compile_options(setExercises
    PRIVATE
    -Wall
    -Wextra
)