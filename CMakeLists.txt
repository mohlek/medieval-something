cmake_minimum_required(VERSION 2.8)

PROJECT(MEDIEVAL_SOMETHING)

set(SOURCE
    src/engine/Window.cpp
    src/engine/Buffer.cpp
    src/engine/VertexArrayObject.cpp
    src/engine/Object.cpp
    src/engine/shader/ShaderLoader.cpp
    src/engine/shader/Shader.cpp
    src/engine/shader/ShaderProgram.cpp
    src/main.cpp
)

set(SHADERS
    src/shader/main.frag
    src/shader/main.vert
)

#########################################################
# FIND OpenGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})
add_definitions(${OPENGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OpenGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND GLEW
#########################################################
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})
if(NOT GLEW_FOUND)
    message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)

#########################################################
# FIND GLFW
#########################################################
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
# find_package(glfw3 3.2 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})
add_definitions(${GLDFW_DEFINITIONS})
if(NOT GLFW_FOUND)
    message(ERROR " GLFW not found!")
endif(NOT GLFW_FOUND)

########################################################
# COPY SHADER
########################################################

foreach(SHADER ${SHADERS})
    message(STATUS "copy shader " ${SHADER})
    file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader/)
endforeach(SHADER)

add_executable(medievalSomething ${SOURCE})

target_link_libraries(medievalSomething ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})
