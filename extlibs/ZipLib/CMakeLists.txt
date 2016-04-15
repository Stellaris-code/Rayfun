project(ZipLib)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file(GLOB_RECURSE HDRS *.h)
file(GLOB_RECURSE SRC_LIST detail/*.cpp extlibs/*.c ZipArchive.cpp ZipArchiveEntry.cpp ZipFile.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC_LIST} ${HDRS})
