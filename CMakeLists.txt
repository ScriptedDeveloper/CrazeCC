cmake_minimum_required(VERSION 3.20)
project(CrazeCC)

set(CMAKE_CXX_STANDARD 20 REQUIRED)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES "parser/*.cpp")

add_executable(CrazeCC "main.cpp" ${SOURCES})
