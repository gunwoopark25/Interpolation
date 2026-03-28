cmake_minimum_required(VERSION 3.20)

project(Interpolation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Interpolation
        main.cpp
        Interpolation.cpp
        Point2D.h
)