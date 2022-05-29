//
// Created on 2022/5/29.
//

#include "drawing_methods.hpp"

using std::swap;

namespace lineAlgorithm {
void drawLine_DigitalDifferentialAnalyzer(int x1, int y1, int x2, int y2, Rasterizer &r) {
    int dx = x2 - x1, dy = y2 - y1;
    ColorType color{1.0f, 1.0f, 1.0f};

    int n_step = std::abs(dx) > std::abs(dy) ? std::abs(dx) : std::abs(dy);
    float delta_x = 1.0f * dx / n_step, delta_y = 1.0f * dy / n_step;
    float x = x1, y = y1;

    for (int i = 0; i < n_step; ++i) {
        x = x + delta_x;
        y = y + delta_y;
        r.setPixel(x, y, color);
    }
}

void drawLine_Bresenham_simple(int x1, int y1, int x2, int y2, Rasterizer &r) {
    //浮点计算累积误差问题
    int dx = x2 - x1, dy = y2 - y1;
    DType accum = 0, slope = 1.0f * dy / dx;
    ColorType color{1.0f, 1.0f, 1.0f};

    int x = x1, y = y1;
    for (int i = x; i < x2; ++i) {
        x++;
        accum += slope;
        if (accum > 0.5) {
            y++;
            accum-=1;
        }
        r.setPixel(x, y, color);
    }
}

void drawLine_Bresenham(int x1, int y1, int x2, int y2, Rasterizer &r) {
    //多分支绘制直线算法
    //斜率过高时对换x y
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
}