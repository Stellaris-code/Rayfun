project(imgui)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE HDRS *.h)
file(GLOB_RECURSE SRC_LIST *.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST} ${HDRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
