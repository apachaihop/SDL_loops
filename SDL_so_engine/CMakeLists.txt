cmake_minimum_required(VERSION 3.26)
project(SDL_so_engine)

add_library(engine SHARED engine.cxx engine.hxx)
target_compile_features(engine PUBLIC cxx_std_17)

find_package(SDL3 REQUIRED)

target_link_libraries(engine PRIVATE SDL3::SDL3-shared)

add_executable(game main.cpp)

target_link_libraries(game PRIVATE engine)
