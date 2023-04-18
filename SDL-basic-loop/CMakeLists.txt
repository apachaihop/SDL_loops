cmake_minimum_required(VERSION 3.5)

project(SDL-loop-event LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

add_executable(SDL-loop-event main.cpp)

target_link_libraries(SDL-loop-event PRIVATE SDL3::SDL3-shared)
