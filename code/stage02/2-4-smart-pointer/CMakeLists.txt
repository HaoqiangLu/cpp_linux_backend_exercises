cmake_minimum_required(VERSION 3.28)

project(smartPointerExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(smartPointerExercises
    main.cpp
    Watcher.cpp
    SmartPointerExercises.cpp
)

target_compile_options(smartPointerExercises
    PRIVATE
    -Wall
    -Wextra
)