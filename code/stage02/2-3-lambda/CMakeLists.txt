cmake_minimum_required(VERSION 3.28)

project(lambdaExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lambdaExercises
    main.cpp
    LambdaExercises.cpp
)

target_compile_options(lambdaExercises
    PRIVATE
    -Wall
    -Wextra
)