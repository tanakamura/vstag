cmake_minimum_required(VERSION 2.6)
add_executable(eval eval.cpp gc.cpp)
add_definitions(-Wall)
target_link_libraries(eval)