cmake_minimum_required(VERSION 3.28)

project(StringExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(StringExercises
    main.cpp
    StringExercises.cpp
)

target_compile_options(StringExercises
    PRIVATE
    -Wall
    -Wextra
)