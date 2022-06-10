//
// Created on 2022/5/28.
//

//#pragma GCC optimize("O1")

#include <iostream>
#include <ctime>
#include <random>

#include "gtest/gtest.h"

#define DEBUG_MODE 1
#include "straight_line.hpp"
//#include "drawing_methods.cpp"

using namespace std;
using namespace line_drawing;

class TimeCompareFixture : public ::testing::Test{
 protected:
    long long int n_repeat;
    default_random_engine e;
    uniform_int_distribution<int> u;

    Rasterizer r;
    int x1,x2,y1,y2;

    virtual void SetUp () {
        n_repeat=100000;
        e=default_random_engine(clock());
        u=uniform_int_distribution<int> (0,899);
        r=Rasterizer(1200,900);

        x1 = u(e), y1 = u(e), x2 = u(e), y2 = u(e);
    }
    virtual void TearDown () {}
};

#define RANDOM 1


TEST_F(TimeCompareFixture, DDA_line) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        if (RANDOM)
            x1 = u(e), y1 = u(e), x2 = u(e), y2 = u(e);
        drawLine_DigitalDifferentialAnalyzer(x1,y1,x2,y2,r);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}

TEST_F(TimeCompareFixture, simple_Bresen) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        if (RANDOM)
            x1 = u(e), y1 = u(e), x2 = u(e), y2 = u(e);
        drawLine_MidpointLineAlgorithm(x1, y1, x2, y2, r);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}


void drawLine_Bresenham1(int x1, int y1, int x2, int y2, Rasterizer &r) {
    //单分支情况下，各个分支性能差异不大

    int dx = x2 - x1, dy = y2 - y1;
    int epsilon = 0;
    ColorType color{1.0f, 1.0f, 1.0f};

    int x = x1, y = y1;
    for (; x <= x2; ++x) {
        if (2 * (epsilon + dy) < dx) {
            epsilon = epsilon + dy;
        }
        else {
            epsilon = epsilon + (dy - dx);
            ++y;
        }
        r.setPixel(x, y, color);
    }
}
TEST_F(TimeCompareFixture, Bresenham1) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        if (RANDOM)
            x1 = u(e), y1 = u(e), x2 = u(e), y2 = u(e);
        drawLine_Bresenham1(x1,y1,x2,y2,r);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}

void drawLine_Bresenham11(int x1, int y1, int x2, int y2, Rasterizer &r) {
    int dx = x2 - x1, dy = y2 - y1;
    int epsilon = 0;
    ColorType color{1.0f, 1.0f, 1.0f};

    int x = x1, y = y1;
    for (; x <= x2; ++x) {
        if ((epsilon + dy)<<1 < dx) {
            epsilon = epsilon+dy;
        }
        else {
            epsilon = epsilon+(dy - dx);
            ++y;
        }
        r.setPixel(x, y, color);
    }
}
TEST_F(TimeCompareFixture, Bresenham11) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        if (RANDOM)
            x1 = u(e), y1 = u(e), x2 = u(e), y2 = u(e);
        drawLine_Bresenham11(x1,y1,x2,y2,r);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}


void drawLine_Bresenham13(int x1, int y1, int x2, int y2, Rasterizer &r) {
    int dx = x2 - x1, dy = y2 - y1;
    int epsilon = 0;
    ColorType color{1.0f, 1.0f, 1.0f};

    int x = x1, y = y1;
    for (; x <= x2; ++x) {
        if (2*(epsilon + dy) < dx) {
            epsilon = epsilon+dy;
        }
        else {
            ++y;
            epsilon = epsilon+(dy - dx);
        }
        r.setPixel(x, y, color);
    }
}
TEST_F(TimeCompareFixture, Bresenham13) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        if (RANDOM)
            x1 = u(e), y1 = u(e), x2 = u(e), y2 = u(e);
        drawLine_Bresenham13(x1,y1,x2,y2,r);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}


void drawLine_Bresenham2(int x1, int y1, int x2, int y2, Rasterizer &r) {
    //多分支
    bool steep=std::abs(y2-y1) > std::abs(x2-x1);
    if (steep) {
        swap(x1,y1);
        swap(x2,y2);
    }
    if (x1 > x2) {
        swap(x1,x2);
        swap(y1,y2);
    }

    int dx = x2 - x1, dy = y2 - y1;
    int epsilon = 0;
    ColorType color{1.0f, 1.0f, 1.0f};

    int x = x1, y = y1;
    for (; x <= x2; ++x) {
        if (2 * (epsilon + dy) < dx) {
            epsilon = epsilon + dy;
        }
        else {
            epsilon = epsilon + (dy - dx);
            ++y;
        }
        if (steep)
            r.setPixel(y, x, color);
        else
            r.setPixel(x, y, color);
    }
}
TEST_F(TimeCompareFixture, Bresenham2) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        if (RANDOM)
            x1 = u(e), y1 = u(e), x2 = u(e), y2 = u(e);
        drawLine_Bresenham2(x1,y1,x2,y2,r);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}

#include <cstdint>
void drawLine_Bresenham21(int x1, int y1, int x2, int y2, Rasterizer &r) {
    //多分支
    bool steep = std::abs(y2 - y1) > std::abs(x2 - x1);
    if (steep) { // x和y直接分支
        swap(x1, y1);
        swap(x2, y2);
    }

    //统一不同象限增长
    int8_t x_inc = x1 <= x2 ? 1 : -1;
    int8_t y_inc = y1 <= y2 ? 1 : -1;

    int dx = std::abs(x2 - x1), dy = std::abs(y2 - y1);
    ColorType color{1.0f, 1.0f, 1.0f};

    int epsilon = 0;
    int x = x1, y = y1;
    for (; x != x2; x += x_inc) {
        if (2 * (epsilon + dy) < dx) {
            epsilon = epsilon + dy;
        }
        else {
            epsilon = epsilon + (dy - dx);
            y += y_inc;
        }

        if (steep)
            r.setPixel(y, x, color);
        else
            r.setPixel(x, y, color);
    }
}

#include <stdexcept>
TEST_F(TimeCompareFixture, Bresenham21) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        if (RANDOM)
            x1 = u(e), y1 = u(e), x2 = u(e), y2 = u(e);

        drawLine_Bresenham21(x1,y1,x2,y2,r);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}

void drawLine_Bresenham22(int x1, int y1, int x2, int y2, Rasterizer &r) {
    //多分支

    int8_t x_inc = x1 <= x2 ? 1 : -1;
    int8_t y_inc = y1 <= y2 ? 1 : -1;

    int x = x1, y = y1;
    int dx = std::abs(x2 - x1), dy = std::abs(y2 - y1);
    int epsilon = 0;
    ColorType color{1.0f, 1.0f, 1.0f};

//    bool steep=std::abs(y2-y1) > std::abs(x2-x1);
    if (dy > dx) { // x和y直接分支
        for (; y != y2; y += y_inc) {
            if (2 * (epsilon + dx) < dy) {
                epsilon = epsilon + dx;
            }
            else {
                epsilon = epsilon + (dx - dy);
                x += x_inc;
            }
            r.setPixel(x,y, color);
        }
    }
    else {
        for (; x != x2; x += x_inc) {
            if (2 * (epsilon + dy) < dx) {
                epsilon = epsilon + dy;
            }
            else {
                epsilon = epsilon + (dy - dx);
                y += y_inc;
            }
            r.setPixel(x, y, color);
        }
    }
}
TEST_F(TimeCompareFixture, Bresenham22) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        if (RANDOM)
            x1 = u(e), y1 = u(e), x2 = u(e), y2 = u(e);
        drawLine_Bresenham22(x1,y1,x2,y2,r);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}


void drawLine_Bresenham20(int x1, int y1, int x2, int y2, Rasterizer &r) {
    //多分支
    bool steep=std::abs(y2-y1) > std::abs(x2-x1);
    if (steep) {
        swap(x1,y1);
        swap(x2,y2);
    }

    int8_t x_inc = x1 <= x2 ? 1 : -1;
    int8_t y_inc = y1 <= y2 ? 1 : -1;

    int dx = std::abs(x2 - x1), dy = std::abs(y2 - y1);
    int epsilon = 0;
    ColorType color{1.0f, 1.0f, 1.0f};

    int x = x1, y = y1;
    for (; x != x2; x += x_inc) {
        epsilon += dy << 1;
        if (epsilon > dx) {
            epsilon -= dx << 1;
            y += y_inc;
        }

        if (steep)
            r.setPixel(y, x, color);
        else
            r.setPixel(x, y, color);
    }
}
TEST_F(TimeCompareFixture, Bresenham20) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        if (RANDOM)
            x1 = u(e), y1 = u(e), x2 = u(e), y2 = u(e);
        drawLine_Bresenham20(x1,y1,x2,y2,r);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}

int main (int argc,char** argv) {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}