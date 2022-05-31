//
// Created on 2022/5/26.
//

#ifndef RASTERIZER_TRIANGLE_HPP
#define RASTERIZER_TRIANGLE_HPP

#include <array>

#include <Eigen/Eigen>

using std::array;
using Eigen::Vector3f;

class Triangle {

public:
    array<Vector3f ,3> vertex_;
    array<Vector3f ,3> color_;

    Triangle();

    void setVertex(int idx, const Vector3f &v) {vertex_[idx] = v;}
    void setColor(int idx, const Vector3f &color) {color_[idx] = color;}
    void setAllColors(const Vector3f &color) {
        setColor(0,color);
        setColor(1,color);
        setColor(2,color);
    }

    Vector3f getColor(int idx) const {return color_[idx];}
};


#endif //RASTERIZER_TRIANGLE_HPP
