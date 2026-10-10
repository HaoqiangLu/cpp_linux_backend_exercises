cmake_minimum_required(VERSION 3.28)

project(http-server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(http-server ${SOURCES})

target_include_directories(http-server
    PRIVATE
    include/
    src/
)

target_link_libraries(http-server PRIVATE pthread)

target_compile_options(http-server
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(test_http_parser
    tests/test_http_parser.cpp
    src/HttpRequest.cpp
    src/HttpResponse.cpp
)
target_include_directories(test_http_parser
    PRIVATE
    include/
    src/
)
target_compile_options(test_http_parser
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
