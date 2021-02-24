cmake_minimum_required(VERSION 3.10)

project(water_simulation VERSION 0.1)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++2a -g3 -O0")

set(SOURCES
    src/generic_image.cc
    src/program.cc
    src/error.cc
    src/main.cc src/callbacks.cc src/object.cc src/camera.cc)

add_executable(${PROJECT_NAME} ${SOURCES})

add_dependencies(${PROJECT_NAME} OpenGL::OpenGL)
include_directories(${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} OpenGL::OpenGL ${GLUT_LIBRARY} GLEW)

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
