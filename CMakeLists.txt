cmake_minimum_required(VERSION 3.6)

add_definitions(-std=c++17)

project(Figures)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(SOURCES
    "canvas.h"
    "canvas.cpp"
    "color.h"
    "figures.cpp"
    "figures.h"
    "vect2d.cpp"
    "vect2d.h"
    "main.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
