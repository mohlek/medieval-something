cmake_minimum_required(VERSION 3.5)

PROJECT(MEDIEVAL_SOMETHING)

file(GLOB_RECURSE src_medieval "src/*.cpp")

add_subdirectory(engine/)

add_executable(medievalSomething ${src_medieval})

target_link_libraries(medievalSomething engine)
