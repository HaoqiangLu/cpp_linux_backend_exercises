cmake_minimum_required(VERSION 3.28)

project(redisExercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(redisExercises
    main.cpp
    RedisClient.cpp
)

target_compile_options(redisExercises
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

find_package(redis++ CONFIG REQUIRED)
target_link_libraries(redisExercises
    PRIVATE
    redis++::redis++_static
)