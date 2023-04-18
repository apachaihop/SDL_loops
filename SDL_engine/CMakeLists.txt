cmake_minimum_required(VERSION 3.5)
project(SDL_engine)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL3 REQUIRED)

add_executable(SDL_engine main.cpp engine.cxx engine.hxx)

target_link_libraries(SDL_engine PRIVATE SDL3::SDL3-static)





