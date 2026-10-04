cmake_minimum_required(VERSION 3.28)

project(packetExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(packetExercises
    main.cpp
    PacketCodec.cpp
)

target_compile_options(packetExercises
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)