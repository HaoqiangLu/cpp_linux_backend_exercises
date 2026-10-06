cmake_minimum_required(VERSION 3.28)

project(mysqlBasic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(mysqlBasic
    main.cpp
    UserRepo.cpp
)

target_compile_options(mysqlBasic
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

find_package(unofficial-libmariadb CONFIG REQUIRED)
target_link_libraries(mysqlBasic
    PRIVATE
    unofficial::libmariadb
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libxcrypt REQUIRED IMPORTED_TARGET libxcrypt)
target_link_libraries(mysqlBasic
    PRIVATE
    PkgConfig::libxcrypt
)