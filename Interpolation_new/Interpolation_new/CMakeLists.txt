cmake_minimum_required(VERSION 3.16)

project(Interpolation LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Debug build
set(CMAKE_BUILD_TYPE Debug)

# Executable
add_executable(Interpolation
    main.cpp
    Interpolation.cpp
)