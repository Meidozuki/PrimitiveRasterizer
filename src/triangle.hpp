//
// Created on 2022/5/26.
//

#ifndef RASTERIZER_TRIANGLE_HPP
#define RASTERIZER_TRIANGLE_HPP

#include <array>
#include <memory>
#include <cassert>
#include <cinttypes>

#include <Eigen/Core>
#include <Eigen/Geometry>

using std::shared_ptr;
using Eigen::Vector3f;
using Array3_Vector3f = std::array<Vector3f ,3>;


class Triangle {
private:
    Array3_Vector3f vertex_;
    Array3_Vector3f normal_;
    Array3_Vector3f color_;

public:

    Triangle();

    //getters
    const Vector3f& vertex(int idx) const {return vertex_[idx];}
    const Vector3f& normal(int idx) const {return normal_[idx];}
    const Vector3f& color(int idx) const {return color_[idx];}

    auto getVertices() const {return vertex_;}
    void setVertex(int idx, const Vector3f &v) {vertex_[idx] = v;}

    void setNormal(int idx,const Vector3f &normal) {normal_[idx] = normal;}
    void setNormals(const std::vector<Vector3f> &normal);
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



};


#endif //RASTERIZER_TRIANGLE_HPP
