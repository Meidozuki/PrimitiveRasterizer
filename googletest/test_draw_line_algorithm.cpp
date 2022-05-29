//
// Created on 2022/5/28.
//

#include "gtest/gtest.h"

#include "rasterizer.cpp"
using namespace lineAlgorithm;

class LineAlgorithmFixture : public ::testing::Test{
 protected:
    Rasterizer r;
    int x1,x2,y1,y2;

    virtual void SetUp () {
        r = Rasterizer(200,200);
        x1 = y1 = 100;
        x2 = y2 = 200;
    }
    virtual void TearDown () {}
};

TEST_F(LineAlgorithmFixture, DDA_line) {
    drawLine_DigitalDifferentialAnalyzer(x1,y1,x2,y2,r);
}

TEST_F(LineAlgorithmFixture, simple_bresenham_line) {
    drawLine_Bresenham_simple(x1,y1,x2,y2,r);
}