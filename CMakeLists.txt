cmake_minimum_required(VERSION 3.23)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lab_04 LANGUAGES CXX)

add_executable(lab_04_run src/main.cpp src/classes.cpp)

add_executable(lab_04_tests tests/test_cases.cpp src/classes.cpp)

target_link_libraries(lab_04_tests PRIVATE gtest gtest_main)