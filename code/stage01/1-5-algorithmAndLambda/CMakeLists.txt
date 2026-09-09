cmake_minimum_required(VERSION 3.28)
project(algorithmExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(algorithmExercises
    main.cpp
    AlgorithmExercises.cpp
)

target_compile_options(algorithmExercises
    PRIVATE
    -Wall
    -Wextra
)