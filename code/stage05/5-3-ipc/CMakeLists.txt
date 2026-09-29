cmake_minimum_required(VERSION 3.28)

project(ipcExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ipcExercises
    main.cpp
    IpcExercises.cpp
)

target_compile_options(ipcExercises
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# target_link_libraries(ipcExercises PRIVATE pthread rt)