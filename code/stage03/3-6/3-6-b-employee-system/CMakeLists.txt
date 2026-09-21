cmake_minimum_required(VERSION 3.28)

project(employeeSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(employeeSystem
    main.cpp
    Employee.cpp
    EmployeeManager.cpp
    Manager.cpp
    Programmer.cpp
)

target_compile_options(employeeSystem
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)