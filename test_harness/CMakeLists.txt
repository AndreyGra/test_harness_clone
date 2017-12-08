cmake_minimum_required(VERSION 3.9)
project(test_harness)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_harness main.cpp TimeKeeper.cpp TimeKeeper.h)