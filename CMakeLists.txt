cmake_minimum_required(VERSION 3.17)
project(Rasterizer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -O1)

#configure_file(header_config.h.in header_config.h)

option(Raster_ObjLoader_primitive OFF "使用自己写的objloader而不是调用现成")

#eigen package
include_directories(3Party/eigen/include/eigen3)

#opencv package
option(OpenCV_STATIC OFF)
find_package(OpenCV REQUIRED
        PATHS 3Party/opencv)
include_directories(3Party/opencv/include)

include_directories(src)
file(GLOB SRC_CPP
        src/*.cpp src/voxel/*.cpp)
list(APPEND SRC_CPP
        main.cpp)
add_executable(Rasterizer ${SRC_CPP})

target_link_libraries(Rasterizer ${OpenCV_LIBS})

#target_include_directories(Rasterizer PUBLIC
#        "${PROJECT_BINARY_DIR}"
#        )

#check opencv dll
file(GLOB DLLS_
        ${PROJECT_BINARY_DIR}/*.dll)
list(LENGTH DLLS_ DLLS_num)
if(DLLS_num EQUAL 0)
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
#add_subdirectory(googletest)