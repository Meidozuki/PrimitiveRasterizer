# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run test1.cpp test_draw_line_algorithm.cpp )
target_link_libraries(Google_Tests_run gtest gtest_main)
target_link_libraries(Google_Tests_run Raster_src_lib)

function(add_gtest_exe exe src)
    add_executable(${exe} ${src})
    target_link_libraries(${exe} gtest gtest_main)
    target_link_libraries(${exe} Raster_src_lib)
endfunction()

add_gtest_exe(gtest_time time_comparation.cpp )
#add_gtest_exe(gtest_time time_comparation_2.cpp )