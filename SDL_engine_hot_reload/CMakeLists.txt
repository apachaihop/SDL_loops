cmake_minimum_required(VERSION 3.25)
project(SDL_engine_hot_reload)

add_executable(engine engine.cxx engine.hxx)
target_compile_features(engine PUBLIC cxx_std_17)

set_target_properties(
        engine PROPERTIES
        ENABLE_EXPORTS 1 # we need to be able to link to exe functions from game
)

find_package(SDL3 REQUIRED)

target_link_libraries(engine PRIVATE SDL3::SDL3-shared)

add_library(game SHARED game.cxx)

target_compile_features(game PUBLIC cxx_std_17)
target_link_libraries(game PRIVATE engine)
