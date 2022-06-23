//
// Created on 2022/6/4.
//

#include "polygon.hpp"

#include <cmath>
#include <iostream>

#define SEE_VERTEX_AND_NORMAL 0

namespace voxel {
//static member
ColorType Mesh::mesh_color = Vector3f(1,1,1);

void Mesh::getTriangles(std::vector<Triangle> &tri_list) {
    const int len = indices_.size();
    for (int i=0;i < len;++i) {
        Triangle tri;
        for (int j=0;j < 3;++j) {
            int vertex_idx = indices_[i][j];
            tri.setVertex(j, vertex_pos_[vertex_idx]);
        }
        tri.setAllColors(Mesh::mesh_color);

        if (!vertex_normal_.empty() && !indices_vn_.empty()) {
            for (int j=0;j < 3;++j) {
                int vn_idx = indices_vn_.at(i)[j];
                tri.setNormal(j, vertex_normal_.at(vn_idx));
            }
        }

        tri_list.push_back(std::move(tri));
    }
}

void Mesh2D::getTriangles(std::vector<Triangle> &tri_list) {
    using std::swap;
    std::function<Vector3f (Vector3f&)> swapAxis;
    if (aligned_axis == axisX) {
        swapAxis = [](Vector3f& v) {
          swap(v.x(),v.z());
          return v;
        };
    }
    else if (aligned_axis == axisY) {
        swapAxis = [](Vector3f& v) {
            swap(v.y(),v.z());
            return v;
        };
    }
    else {
        swapAxis = [](const Vector3f& v) {return v;};
    }

    const int len = indices_.size();
    for (int i=0;i < len;++i) {
        Triangle tri;
        for (int j=0;j < 3;++j) {
            int vertex_idx = indices_[i][j];
            //更改xyz顺序
            Vector3f vert=vertex_pos_[vertex_idx];
            tri.setVertex(j, swapAxis(vert));
        }
        tri.setAllColors(Mesh::mesh_color);

        if (!vertex_normal_.empty() && !indices_vn_.empty()) {
            for (int j=0;j < 3;++j) {
                int vn_idx = indices_vn_.at(i)[j];
                //更改normal
                Vector3f normal = vertex_normal_.at(vn_idx);
                tri.setNormal(j, swapAxis(normal));
            }
        }

        tri_list.push_back(std::move(tri));
    }

}

Rectangle::Rectangle(float top, float left, float bottom, float right,
                     float z) {
    z_=z;

    vertex_pos_.emplace_back(left,top,z_);
    vertex_pos_.emplace_back(left,bottom,z_);
    vertex_pos_.emplace_back(right,top,z_);
    vertex_pos_.emplace_back(right,bottom,z_);

    indices_.emplace_back(0,1,2);
    indices_.emplace_back(1,2,3);
}

Cone::Cone(int edges, const Eigen::Array3f& center, float radius, float tip_relative) {
    assert (edges >= 3);
#if GENERAL_DEBUG_MODE
    if (std::abs(tip_relative) < 1e-5) {
        throw std::invalid_argument("Cone received a zero *tip_relative* arg");
    }
#endif

    float axis_coef = tip_relative > 0 ? 1 : -1;
    float step_angle = 2.0 * M_PI / edges;
    set_tip(Eigen::Array3f(0,tip_relative,0) * radius + center);
    vertex_pos_.emplace_back(tip_);

    for (int i=0;i < edges;++i) {
        float angle = step_angle * i;
        Eigen::Array3f vert(std::cos(angle),0,std::sin(angle));
        vertex_pos_.emplace_back(vert * radius + center);
    }

    if (SEE_VERTEX_AND_NORMAL) {
        std::cout << "vertices\n";
        for (auto v:vertex_pos_) {
            std::cout << (v-Vector3f(0,0.5,0)).transpose().array()/0.5 << std::endl;
        }
        std::cout << "--------\n";

    }

    //n-2个底面
    Vector3f bottom_normal(0,-1,0);
    vertex_normal_.emplace_back(bottom_normal * axis_coef);
    for (int i=2;i < edges;++i) {
        indices_.emplace_back(1,i,i+1);
        indices_vn_.emplace_back(Vector3i::Constant(0));
    }
    //n个侧面
    for (int i=0;i < edges;++i) {
        int idx_a = i, idx_b = i + 1, idx_vn = i+1;
        if (i == 0) idx_a = edges, idx_b = 1;

        indices_.emplace_back(0,idx_a,idx_b);
        Vector3f OA = vertex_pos_.at(idx_a) - vertex_pos_[0];
        Vector3f OB = vertex_pos_.at(idx_b) - vertex_pos_[0];
        const Vector3f &&normal = OB.cross(OA).normalized();
        if (SEE_VERTEX_AND_NORMAL)
            std::cout << normal.transpose() << std::endl;

        vertex_normal_.emplace_back(normal * axis_coef);
        //由于编号0被底面占了，计数从1开始
        assert(idx_vn >= 1);
        indices_vn_.emplace_back(Vector3i::Constant(idx_vn));
    }
}

Cube::Cube(float x1, float y1, float z1, float x2, float y2, float z2) {

    vertex_pos_.emplace_back(x1,y1,z1);
    vertex_pos_.emplace_back(x1,y2,z1);
    vertex_pos_.emplace_back(x2,y1,z1);
    vertex_pos_.emplace_back(x2,y2,z1);
    vertex_pos_.emplace_back(x1,y1,z2);
    vertex_pos_.emplace_back(x1,y2,z2);
    vertex_pos_.emplace_back(x2,y1,z2);
    vertex_pos_.emplace_back(x2,y2,z2);

    vertex_normal_.emplace_back(0,0,-1);
    vertex_normal_.emplace_back(0,0, 1);
    vertex_normal_.emplace_back(0,-1,0);
    vertex_normal_.emplace_back(0, 1,0);
    vertex_normal_.emplace_back(-1,0,0);
    vertex_normal_.emplace_back( 1,0,0);

    //bottom
    indices_.emplace_back(1,2,0);
    indices_.emplace_back(1,2,3);
    indices_vn_.emplace_back(Vector3i::Constant(0));
    indices_vn_.emplace_back(Vector3i::Constant(0));
    //front
    indices_.emplace_back(2,4,0);
    indices_.emplace_back(2,4,6);
    indices_vn_.emplace_back(Vector3i::Constant(2));
    indices_vn_.emplace_back(Vector3i::Constant(2));
    //left
    indices_.emplace_back(1,4,0);
    indices_.emplace_back(1,4,5);
    indices_vn_.emplace_back(Vector3i::Constant(4));
    indices_vn_.emplace_back(Vector3i::Constant(4));
    //top
    indices_.emplace_back(5,6,4);
    indices_.emplace_back(5,6,7);
    indices_vn_.emplace_back(Vector3i::Constant(1));
    indices_vn_.emplace_back(Vector3i::Constant(1));
    //back
    indices_.emplace_back(3,5,1);
    indices_.emplace_back(3,5,7);
    indices_vn_.emplace_back(Vector3i::Constant(3));
    indices_vn_.emplace_back(Vector3i::Constant(3));
    //right
    indices_.emplace_back(3,6,2);
    indices_.emplace_back(3,6,7);
    indices_vn_.emplace_back(Vector3i::Constant(5));
    indices_vn_.emplace_back(Vector3i::Constant(5));
}
}