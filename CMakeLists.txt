cmake_minimum_required(VERSION 3.10)

project(22127384)

set(OpenCV_DIR "C:/opencv/build")
find_package(OpenCV REQUIRED)

if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found! Please check OpenCV_DIR path.")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include_directories(${CMAKE_SOURCE_DIR}/include)
# include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} source/main.cpp)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
