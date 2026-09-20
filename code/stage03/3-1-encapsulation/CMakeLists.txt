cmake_minimum_required(VERSION 3.28)

project(encapsulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(encapsulation
    main.cpp
    Rectangle.cpp
    BankAccount.cpp
    Student.cpp
)

target_compile_options(encapsulation
    PRIVATE
    -Wall
    -Wextra
)