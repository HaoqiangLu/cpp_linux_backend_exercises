cmake_minimum_required(VERSION 3.28)
project(proA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(proA
    main.cpp
    GradeManager.cpp
)

target_compile_options(proA
    PRIVATE
    -Wall
    -Wextra
)