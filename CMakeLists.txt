cmake_minimum_required(VERSION 3.21)

PROJECT(MEDIEVAL_SOMETHING)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE src_medieval "src/*.cpp")

add_subdirectory(engine/)

add_executable(medievalSomething ${src_medieval})

target_include_directories(medievalSomething PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(medievalSomething engine gl3w)
