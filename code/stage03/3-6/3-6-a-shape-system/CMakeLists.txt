cmake_minimum_required(VERSION 3.28)

project(shapeSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(shapeSystem
    main.cpp
    Circle.cpp
    Rectangle.cpp
    Triangle.cpp
    ShapeManager.cpp
)

target_compile_options(shapeSystem
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
