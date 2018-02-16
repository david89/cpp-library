cmake_minimum_required (VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project (string_view_test)
add_library(string_view string_view.cc)
add_executable(string_view_test string_view_test.cc)

target_link_libraries(string_view_test string_view pthread gtest_main gtest)
