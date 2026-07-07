cmake_minimum_required(VERSION 3.14)
project(calc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Strict warnings — treat warnings as errors for clean builds
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Calculator executable
add_executable(calc
    src/main.cpp
    src/tokenizer.cpp
    src/parser.cpp
)

target_include_directories(calc PRIVATE src)

# Test executable
enable_testing()

add_executable(test_calculator
    tests/test_calculator.cpp
    src/tokenizer.cpp
    src/parser.cpp
)

target_include_directories(test_calculator PRIVATE src)

add_test(NAME calculator_tests COMMAND test_calculator)
