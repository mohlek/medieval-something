cmake_minimum_required(VERSION 2.8)

PROJECT(MEDIEVAL_SOMETHING)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

set(SOURCE
    src/engine/Window.cpp
    src/engine/Buffer.cpp
    src/engine/VertexArrayObject.cpp
    src/engine/Object.cpp
    src/engine/Camera.cpp
    src/engine/shader/ShaderLoader.cpp
    src/engine/shader/Shader.cpp
    src/engine/shader/ShaderProgram.cpp
    src/engine/model/3ds/Model3DS.cpp
    src/game/SkyBox.cpp
    src/game/shader/MainShader.cpp
    src/game/shader/SkyBoxShader.cpp
    src/main.cpp
)

set(SHADERS
    src/game/shader/main.frag
    src/game/shader/main.vert
    src/game/shader/skybox.frag
    src/game/shader/skybox.vert
)

set(RESOURCES
    resource/dragon.3ds
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
# FIND LIB3DS
########################################################
find_package(Lib3ds)
include_directories(${LIB3DS_INCLUDE_DIRS})
link_directories(${LIB3DS_LIBRARY_DIRS})
if(NOT LIB3DS_FOUND)
    message(ERROR " LIB3DS not found!")
endif(NOT LIB3DS_FOUND)


########################################################
# COPY SHADER
########################################################

foreach(SHADER ${SHADERS})
    message(STATUS "copy shader " ${SHADER})
    file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader/)
endforeach(SHADER)

message(STATUS "copy resources")
file(COPY ${RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resource/)

add_executable(medievalSomething ${SOURCE})

target_link_libraries(medievalSomething ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${LIB3DS_LIBRARY})
