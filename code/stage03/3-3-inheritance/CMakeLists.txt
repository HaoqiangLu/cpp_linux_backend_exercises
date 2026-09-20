cmake_minimum_required(VERSION 3.28)

project(inheritance LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(inheritance
    main.cpp
    Animal.cpp
    Dog.cpp
    Cat.cpp
    Shape.cpp
    Circle.cpp
    Rectangle.cpp
    Triangle.cpp
)

target_compile_options(inheritance
    PRIVATE
    -Wall
    -Wextra
)
