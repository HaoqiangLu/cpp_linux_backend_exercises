cmake_minimum_required(VERSION 3.28)

project(sockoptExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(sockoptExercises
    main.cpp
    SockOptExercises.cpp
)

target_compile_options(sockoptExercises
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)