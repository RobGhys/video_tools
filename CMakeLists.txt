cmake_minimum_required(VERSION 3.27)
project(video_tools)

set(CMAKE_CXX_STANDARD 23)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(video_tools main.cpp)
target_link_libraries(video_tools ${OpenCV_LIBS})