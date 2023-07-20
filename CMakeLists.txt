cmake_minimum_required(VERSION 3.20)
project(CrazeCC)

set(CMAKE_CXX_STANDARD 20 REQUIRED)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

file(GLOB SOURCES "parser/*.cpp")

add_executable(CrazeCC "main.cpp" ${SOURCES})
set_property(TARGET CrazeCC PROPERTY COMPILE_WARNING_AS_ERROR ON)
