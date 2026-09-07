cmake_minimum_required(VERSION 3.28)

project(MapExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MapExercises
    main.cpp
    MapExercises.cpp
    PhoneBook.cpp
    StudentLookup.cpp
)

target_compile_options(MapExercises
    PRIVATE
    -Wall
    -Wextra
)