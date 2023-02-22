# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(googletest-release-1.12.1)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#使用opencv dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS -O1)

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run test_mesh2d.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)
target_link_libraries(Google_Tests_run Raster_src_lib)


function(add_gtest_exe exe src)
    add_executable(${exe} ${src})
    target_link_libraries(${exe} gtest gtest_main)
    target_link_libraries(${exe} Raster_src_lib)
endfunction()

add_gtest_exe(test3d test_3d.cpp )
#add_gtest_exe(gtest_time time_comparation_2.cpp )
