project(RayFun)
cmake_minimum_required(VERSION 2.8)

include_directories(include)
include_directories(src/gpu)
include_directories(extlibs)
include_directories(extlibs/ChaiScript/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -ldl -g -Og")
#  -ffast-math -flto -ftree-vectorize -fopenmp -march=native -mtune=native -msse3

add_subdirectory(extlibs/ZipLib)
add_subdirectory(extlibs/imgui)

file(GLOB_RECURSE HDRS include/*.hpp include/*.tpp include/*.inl include/*.ui)
file(GLOB_RECURSE SRC_LIST src/*.cpp)

option(MSGBOX "Use message box for error reporting" ON)
option(USE_SDL2_FOR_MSGBOX "Use SDL2 instead of FLTK for message box" ON)

find_package(Boost COMPONENTS system program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(SFML 2 REQUIRED system window graphics audio)
include_directories(${SFML_INCLUDE_DIR})

find_package(Thor REQUIRED)
include_directories(${Thor_INCLUDE_DIR})

find_package(Jsoncpp REQUIRED)
include_directories(${JSONCPP_INCLUDE_DIR})

if (MSGBOX)
    if (USE_SDL2_FOR_MSGBOX)
        find_package(SDL2 REQUIRED)
        if (SDL2_FOUND)
            include_directories(${SDL2_INCLUDE_DIR})
        endif()
    else()
        find_package(FLTK)
        if (FLTK_FOUND)
            include_directories(${FLTK_INCLUDE_DIR})
        endif ()
    endif()
endif()

configure_file(include/cmake/config.hpp.in ${PROJECT_SOURCE_DIR}/include/cmake/config.hpp @ONLY)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HDRS})

target_link_libraries(${PROJECT_NAME} ZipLib)
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${THOR_LIBRARY})
if (MSGBOX)
    if (USE_SDL2_FOR_MSGBOX)
        if (SDL2_FOUND)
            target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
        endif()
    else()
        if (FLTK_FOUND)
            target_link_libraries(${PROJECT_NAME} ${FLTK_LIBRARY})
        endif ()
    endif()
endif()
