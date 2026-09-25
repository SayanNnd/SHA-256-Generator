cmake_minimum_required(VERSION 3.20)
project(sha256 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

add_executable(sha256 main.cpp sha256.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(sha256 PRIVATE -O3 -march=native)

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(sha256 PRIVATE -g -Wall -Wextra -Werror)
endif()