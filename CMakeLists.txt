cmake_minimum_required(VERSION 3.19)
project(Rasterizer)
set(CMAKE_CXX_STANDARD 17)

#configure_file(header_config.h.in header_config.h)
#eigen package
include_directories(3Party/eigen/include/eigen3)

#opencv package
option(OpenCV_STATIC OFF)
find_package(OpenCV REQUIRED
        PATHS 3Party/opencv)
include_directories(3Party/opencv/include)

include_directories(src)
file(GLOB SRC_CPP
        src/*.cpp)
list(APPEND SRC_CPP
        main.cpp)
add_executable(Rasterizer ${SRC_CPP})

target_link_libraries(Rasterizer ${OpenCV_LIBS})

target_include_directories(Rasterizer PUBLIC
        "${PROJECT_BINARY_DIR}"
        )