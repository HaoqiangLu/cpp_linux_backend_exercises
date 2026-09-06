cmake_minimum_required(VERSION 3.28)
project(vectorExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vectorExercises
    main.cpp
    VectorExercises.cpp
    GradeBook.cpp
)

target_compile_options(vectorExercises
    PRIVATE
    -Wall
    -Wextra
)