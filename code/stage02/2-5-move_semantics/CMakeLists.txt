cmake_minimum_required(VERSION 3.28)

project(moveSemantics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(moveSemantics
    main.cpp
    Resource.cpp
    MyString.cpp
    MoveExercises.cpp
)

target_compile_options(moveSemantics
    PRIVATE
    -Wall
    -Wextra
)