//
// Created on 2022/5/27.
//

#include "rasterizer.hpp"

#include <cmath>

#include "drawing_methods.hpp"

using namespace lineAlgorithm;
void Rasterizer::drawLine(Eigen::Vector3f begin,Eigen::Vector3f end) {
//    drawLine_DigitalDifferentialAnalyzer(begin.x(),begin.y(),end.x(),end.y(),*this);
    drawLine_Bresenham(begin.x(),begin.y(),end.x(),end.y(),*this);

}