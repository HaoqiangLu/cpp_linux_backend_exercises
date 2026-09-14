cmake_minimum_required(VERSION 3.28)

project(autoExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(autoExercises
    main.cpp
    AutoExercises.cpp
)

target_compile_options(autoExercises
    PRIVATE
    -Wall
    -Wextra
)