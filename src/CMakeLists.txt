project(Raster_src_lib)

set(HEADER_FILES
        rasterizer.hpp
        )

set(SOURCE_FILES
        rasterizer.cpp
        )

add_library(Raster_src_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})