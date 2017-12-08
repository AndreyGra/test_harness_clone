cmake_minimum_required(VERSION 3.5)
project(test_harness)

find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
set(SOURCE_FILES main.cpp)
add_executable(test_harness main.cpp TimeKeeper.cpp TimeKeeper.h ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(test_harness ${OpenCV_LIBS})