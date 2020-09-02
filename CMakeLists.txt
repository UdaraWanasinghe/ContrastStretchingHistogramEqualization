cmake_minimum_required(VERSION 3.16)
project(contrast_stretching_histogram_equalization)

set(CMAKE_CXX_STANDARD 14)

add_executable(contrast_stretching_histogram_equalization main.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})