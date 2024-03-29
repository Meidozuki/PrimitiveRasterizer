cmake_minimum_required(VERSION 3.16)
project(Rasterizer)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS -O1)

message(STATUS "OS: ${CMAKE_SYSTEM}")

configure_file(config.h.in config.h)

option(Raster_ObjLoader_primitive OFF "使用自己写的objloader而不是调用现成")

set(VBAO_DIR "D:/coding/git/VBAO/Lib_VBao" CACHE PATH "")
include_directories(${VBAO_DIR})

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
#    set(THIRD_LIB_PATH ${PROJECT_SOURCE_DIR}/3Party)
    set(THIRD_LIB_PATH D:/coding/3rdParty/install)
    #eigen package
    include_directories(${THIRD_LIB_PATH}/eigen/include/eigen3)
    #opencv package
    find_package(OpenCV REQUIRED
            PATHS ${THIRD_LIB_PATH}/opencv)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(eigen3)
    include_directories(/usr/include/eigen3/)
    find_package(OpenCV 4.5 REQUIRED
            PATHS ${CMAKE_SOURCE_DIR}/3Party/linux-build/opencv-install)
endif()
message(STATUS ${OpenCV_VERSION})
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(src)
file(GLOB SRC_CPP
        src/*.cpp src/voxel/*.cpp
        src/MVVM/*/*.cpp)
list(APPEND SRC_CPP
        main.cpp)
add_executable(Rasterizer ${SRC_CPP})

add_executable(test debug_tool/test_feature.cpp)

target_link_libraries(Rasterizer ${OpenCV_LIBS})

target_include_directories(Rasterizer PUBLIC
        "${PROJECT_BINARY_DIR}"
        )

#预编译头
target_precompile_headers(Rasterizer
        PRIVATE src/precompile.h
        )

#check opencv dll
set(OpenCV_STATIC ON)
file(GLOB DLLS_
        ${PROJECT_BINARY_DIR}/*.dll)
list(LENGTH DLLS_ DLLS_num)
if(CMAKE_SYSTEM_NAME MATCHES "Windows" AND NOT OpenCV_STATIC AND DLLS_num EQUAL 0)
    message(WARNING "The exe needs opencv dlls but none were found")

    #auto copy opencv(world) dll
    file(GLOB OpenCV_DLLs
            ${_OpenCV_LIB_PATH}/*.dll
            )
    message(STATUS "Found OpenCV DLLs in lib: " ${OpenCV_DLLs})

    list(LENGTH OpenCV_DLLs OpenCV_DLL_nums)
    if (OpenCV_DLL_nums EQUAL 1 AND
            ${OpenCV_DLLs} MATCHES "libopencv_world[0-9]*.dll$"
            )
        message(STATUS "Copying opencv_world to exe directory")
        file(COPY ${OpenCV_DLLs}
                DESTINATION ${PROJECT_BINARY_DIR})
    endif()
    unset(OpenCV_DLL_nums)
    unset(OpenCV_DLLs)
    #auto copy opencvworld end
endif()
unset(DLLS_num)
unset(DLLS_)

add_subdirectory(src)
add_subdirectory(unittest)
