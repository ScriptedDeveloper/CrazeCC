cmake_minimum_required(VERSION 3.20)
project(CrazeCC)
include(GoogleTest)
set(CMAKE_CXX_STANDARD 20 REQUIRED)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest)

enable_testing()

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

file(GLOB SOURCES "parser/*.cpp" "exception/*.cpp" "code_gen/*.cpp" "preprocessor/*.cpp", "compiler/*.cpp" "tests/*.cpp")

add_executable(CrazeCC "main.cpp" ${SOURCES})
add_executable(compiler_tests "tests/test.cpp")
target_link_libraries(compiler_tests GTest::gtest_main)
gtest_discover_tests(compiler_tests)
target_link_libraries(CrazeCC GTest::gtest_main)
set_property(TARGET CrazeCC PROPERTY COMPILE_WARNING_AS_ERROR ON)
