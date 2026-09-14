cmake_minimum_required(VERSION 3.28)

project(taskManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(taskManager
    main.cpp
    TaskManager.cpp
)

target_compile_options(taskManager
    PRIVATE
    -Wall
    -Wextra
)