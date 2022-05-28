//
// Created on 2022/5/27.
//

#include "gtest/gtest.h"

TEST(Simple_test,test01) {
    ASSERT_NE(0,1);
}

#include "rasterizer.hpp"

TEST(Rast_test,setPixel_x_outOfRange) {
    Rasterizer t;
    Eigen::Vector3f color{0,0,0};
    ASSERT_THROW(t.setPixel(-1,0,color),
                 std::out_of_range);
}
TEST(Rast_test,setPixel_y_outOfRange) {
    Rasterizer t;
    Eigen::Vector3f color{0,0,0};
    ASSERT_THROW(t.setPixel(0,-1,color),
                 std::out_of_range);
}

TEST(Rast_test,rasterizer_init) {
    Rasterizer r(30,40);
    EXPECT_EQ(r.width(), 40);
    EXPECT_EQ(r.height(), 30);

    auto [rows,cols] = r.get_framebuffer_shape();
    EXPECT_EQ(cols, 40);
    EXPECT_EQ(rows, 30);
}

int main (int argc,char** argv) {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}