cmake_minimum_required(VERSION 3.10)

project(CV01)

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
find_package(OpenCV REQUIRED)

if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found! Please check OpenCV_DIR path.")
endif()

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Fp\"${CMAKE_BINARY_DIR}/stdafx.pch\"")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Yc\"stdafx.h\"")

add_library(precompiled STATIC src/stdafx.cpp)
add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} precompiled)
