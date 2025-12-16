cmake_minimum_required(VERSION 3.28)
project(PhysicsEngine LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB SOURCES "src/*.cpp")
add_executable(PhysicsEngine ${SOURCES})

target_compile_features(PhysicsEngine PRIVATE cxx_std_17)
target_link_libraries(PhysicsEngine PRIVATE SFML::Graphics)
