cmake_minimum_required(VERSION 3.28)

project(heartbeat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(heartbeat
    main.cpp
    HeartbeatServer.cpp
)

target_compile_options(heartbeat
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)