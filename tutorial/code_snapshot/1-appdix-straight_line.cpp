//
// Created on 2022/5/29.
//

#include "straight_line.hpp"

using std::swap;

namespace line_drawing {
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

void drawLine_MidpointLineAlgorithm(int x1, int y1, int x2, int y2, Rasterizer &r) {
    //浮点计算累积误差问题

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
    DType accum = 0, slope = 1.0f * dy / dx;
    ColorType color{1.0f, 1.0f, 1.0f};

    int x = x1, y = y1;
    for (; x != x2; x += x_inc) {
        accum += slope;
        if (accum > 0.5) {
            y += y_inc;
            accum-=1;
        }

        if (steep)
            r.setPixel(y, x, color);
        else
            r.setPixel(x, y, color);
    }
}

void drawLine_Bresenham(int x1, int y1, int x2, int y2, Rasterizer &r) {
    //多分支绘制直线算法
    //编译器-O0和-O1下不同写法的浮点方法和该方法，在单线程下性能并无显著差异
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
//上方为直接绘制，未考虑z buffer

void drawLine(const Line &line,Rasterizer r) {
    int x1 = line.begin.x(), y1 = line.begin.y(), z1 = line.begin.z();
    int x2 = line.end.x(), y2 = line.end.y(), z2 = line.end.z();

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

    int epsilon = 0;
    int x = x1, y = y1, x_range = x2 - x1;
    for (; x != x2; x += x_inc) {
        if (2 * (epsilon + dy) < dx) {
            epsilon = epsilon + dy;
        }
        else {
            epsilon = epsilon + (dy - dx);
            y += y_inc;
        }


        Rasterizer::ZBufferType cur_depth = r.getZBuffer(x,y);
        float interpolate_ratio = 1.0*(x-x1)/x_range;
        float z = interpolate_ratio * z1 + (1-interpolate_ratio) * z2;
        if (z > cur_depth)
            continue;

        r.setDepth(x,y,z);

        if (steep)
            r.setPixel(y, x, line.color);
        else
            r.setPixel(x, y, line.color);
    }
}

}