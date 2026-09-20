cmake_minimum_required(VERSION 3.28)

project(ctor_dtor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ctor_dtor
    main.cpp
    DynArray.cpp
    Lifecycle.cpp
    MyString.cpp
)

target_compile_options(ctor_dtor
    PRIVATE
    -Wall
    -Wextra
)
