//
// Created on 2022/5/26.
//

#ifndef RASTERIZER_TRIANGLE_HPP
#define RASTERIZER_TRIANGLE_HPP

#include <array>
#include <memory>
#include <cassert>

#include <Eigen/Eigen>

using std::array;
using std::shared_ptr;
using Eigen::Vector3f;

class Triangle;
typedef shared_ptr<Triangle> TrianglePtr;

class Triangle {

public:
    array<Vector3f ,3> vertex_;
    array<Vector3f ,3> color_;
    array<Vector3f ,3> normal_;

    Triangle();

    void setVertex(int idx, const Vector3f &v) {vertex_[idx] = v;}
    void setColor(int idx, const Vector3f &color) {
        assert(color[0] >= 0 && color[0] <= 1 && \
            color[1] >= 0 && color[1] <= 1 && \
            color[2] >= 0 && color[2] <= 1 );
        color_[idx] = color;
    }
    void setAllColors(const Vector3f &color) {
        setColor(0,color);
        setColor(1,color);
        setColor(2,color);
    }

    Vector3f getColor(int idx) const {return color_[idx];}

    friend TrianglePtr createTriangle() {
        TrianglePtr ptr = std::make_shared<Triangle>();
        return ptr;
    }
};


#endif //RASTERIZER_TRIANGLE_HPP
