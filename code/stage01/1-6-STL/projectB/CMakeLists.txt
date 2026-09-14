cmake_minimum_required(VERSION 3.28)

project(wordFrequency LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wordFrequency
    main.cpp
    WordFrequency.cpp
)

target_compile_options(wordFrequency
    PRIVATE
    -Wall
    -Wextra
)