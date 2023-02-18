project(Raster_src_lib)

file(GLOB_RECURSE HEADER_FILES
        *.hpp
        )

file(GLOB_RECURSE SOURCE_FILES
        *.cpp
        )


add_library(Raster_src_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})

#imageio要用到config.h
target_include_directories(Raster_src_lib PUBLIC ${CMAKE_BINARY_DIR})


#opencv package
find_package(OpenCV REQUIRED
        PATHS ${THIRD_LIB_PATH}/opencv)
set(OpenCV_STATIC ON)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Raster_src_lib ${OpenCV_LIBS})
