//
// Created on 2022/5/27.
//

#include "rasterizer.hpp"

#include <cmath>

void drawLine_DigitalDifferentialAnalyzer(int x1,int y1,int x2,int y2,Rasterizer &r) {
    int dx = x2 - x1, dy = y2 - y1;
    ColorType color{1.0f,1.0f,1.0f};

    int n_step = std::abs(dx) > std::abs(dy) ? std::abs(dx) : std::abs(dy);
    float delta_x = dx / n_step , delta_y = dy / n_step;
    float x = x1, y = y1;

    for (int i=0;i < n_step;++i) {
        x = x + delta_x;
        y = y + delta_y;
        r.setPixel(x,y,color);
    }
}

void Rasterizer::drawLine(Eigen::Vector3f begin,Eigen::Vector3f end) {
    drawLine_DigitalDifferentialAnalyzer(begin.x(),begin.y(),end.x(),end.y(),*this);

}