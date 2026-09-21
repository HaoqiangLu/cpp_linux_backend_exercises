cmake_minimum_required(VERSION 3.28)

project(polymorphism LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(polymorphism
    main.cpp
    Circle.cpp
    ConsoleLogger.cpp
    Developer.cpp
    Employee.cpp
    FileLogger.cpp
    Manager.cpp
    PolyDemo.cpp
    Rectangle.cpp
    Shape.cpp
    Triangle.cpp
)

target_compile_options(polymorphism
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)