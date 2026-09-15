cmake_minimum_required(VERSION 3.28)

project(rangeForExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rangeForExercises
    main.cpp
    RangeForExercises.cpp
)

target_compile_options(rangeForExercises
    PRIVATE
    -Wall
    -Wextra
)