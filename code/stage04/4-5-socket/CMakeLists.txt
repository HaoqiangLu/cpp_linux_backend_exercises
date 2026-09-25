cmake_minimum_required(VERSION 3.28)

project(socketExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(socketExercises
    main.cpp
    ChatServer.cpp
    CmdServer.cpp
    EchoClient.cpp
    EchoServer.cpp
    HttpServer.cpp
    MultiClientServer.cpp
)

target_compile_options(socketExercises
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)