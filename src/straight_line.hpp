//
// Created on 2022/5/29.
//

#ifndef MAIN_CPP_SRC_STRAIGHT_LINE_HPP_
#define MAIN_CPP_SRC_STRAIGHT_LINE_HPP_

#include <iostream>

#include <Eigen/Core>

#include "rasterizer.hpp"

struct Line {
    Eigen::Vector3f begin, end;
    ColorType color;
};

namespace line_drawing {
void drawLine(const Line &line,Rasterizer r);

void drawLine_DigitalDifferentialAnalyzer(int x1, int y1, int x2, int y2, Rasterizer &r);
void drawLine_MidpointLineAlgorithm(int x1, int y1, int x2, int y2, Rasterizer &r);
void drawLine_Bresenham(int x1, int y1, int x2, int y2, Rasterizer &r);
}

#endif //MAIN_CPP_SRC_STRAIGHT_LINE_HPP_
