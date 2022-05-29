//
// Created on 2022/5/29.
//

#ifndef MAIN_CPP_SRC_DRAWING_METHODS_HPP_
#define MAIN_CPP_SRC_DRAWING_METHODS_HPP_

#include <iostream>

#include "rasterizer.hpp"

namespace line_drawing {
void drawLine_DigitalDifferentialAnalyzer(int x1, int y1, int x2, int y2, Rasterizer &r);
void drawLine_MidpointLineAlgorithm(int x1, int y1, int x2, int y2, Rasterizer &r);
void drawLine_Bresenham(int x1, int y1, int x2, int y2, Rasterizer &r);
}

#endif //MAIN_CPP_SRC_DRAWING_METHODS_HPP_
