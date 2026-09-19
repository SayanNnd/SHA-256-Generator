cmake_minimum_required(VERSION 3.20)
project(sha256_app CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sha256_app main.cpp sha256.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(sha256_app PRIVATE -O3 -march=native)

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(sha256_app PRIVATE -g -Wall -Wextra)
endif()