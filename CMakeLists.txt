project(RayFun)
cmake_minimum_required(VERSION 2.8)

include_directories(include)
include_directories(extlibs)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(extlibs/ZipLib)
add_subdirectory(extlibs/ChaiScript)

file(GLOB_RECURSE HDRS include/*.hpp)
file(GLOB_RECURSE SRC_LIST src/*.cpp)

find_package(Boost COMPONENTS system program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(SFML 2 REQUIRED system window graphics audio)
include_directories(${SFML_INCLUDE_DIR})

find_package(Thor REQUIRED)
include_directories(${Thor_INCLUDE_DIR})

find_package(Jsoncpp REQUIRED)
include_directories(${Jsoncpp_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HDRS})

target_link_libraries(${PROJECT_NAME} ZipLib)
target_link_libraries(${PROJECT_NAME} Chaiscript)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${Jsoncpp_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${Thor_LIBRARIES})
