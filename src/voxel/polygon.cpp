//
// Created on 2022/6/4.
//

#include "polygon.hpp"

#include <cmath> //提供m_pi
#include <iostream>
#include <algorithm>

#include <Eigen/Geometry>
#include <utility>

#define SEE_VERTEX_AND_NORMAL 0

namespace voxel {

//void Mesh2D::getTriangles(std::vector<Triangle> &tri_list) {
//    const int len = indices_.size();
//    for (int i=0;i < len;++i) {
//        Triangle tri;
//        for (int j=0;j < 3;++j) {
//            int vertex_idx = indices_[i][j];
//            tri.setVertex(j, vertex_pos_[vertex_idx]);
//        }
//        tri.setAllColors(Mesh2D::mesh_color);
//
//        if (!vertex_normal_.empty() && !indices_vn_.empty()) {
//            for (int j=0;j < 3;++j) {
//                int vn_idx = indices_vn_.at(i)[j];
//                tri.setNormal(j, vertex_normal_.at(vn_idx));
//            }
//        }
//
//        tri_list.push_back(std::move(tri));
//    }
//}
//
//void Mesh2D::getTriangles(std::vector<Triangle> &tri_list) {
//    using std::swap;
//    std::function<Vector3f (Vector3f&)> swapAxis;
//    if (aligned_axis == axisX) {
//        swapAxis = [](Vector3f& v) {
//          swap(v.x(),v.z());
//          return v;
//        };
//    }
//    else if (aligned_axis == axisY) {
//        swapAxis = [](Vector3f& v) {
//            swap(v.y(),v.z());
//            return v;
//        };
//    }
//    else {
//        swapAxis = [](const Vector3f& v) {return v;};
//    }
//
//    const int len = indices_.size();
//    for (int i=0;i < len;++i) {
//        Triangle tri;
//        for (int j=0;j < 3;++j) {
//            int vertex_idx = indices_[i][j];
//            //更改xyz顺序
//            Vector3f vert=vertex_pos_[vertex_idx];
//            tri.setVertex(j, swapAxis(vert));
//        }
//        tri.setAllColors(Mesh2D::mesh_color);
//
//        if (!vertex_normal_.empty() && !indices_vn_.empty()) {
//            for (int j=0;j < 3;++j) {
//                int vn_idx = indices_vn_.at(i)[j];
//                //更改normal
//                Vector3f normal = vertex_normal_.at(vn_idx);
//                tri.setNormal(j, swapAxis(normal));
//            }
//        }
//
//        tri_list.push_back(std::move(tri));
//    }
//
//}

//------
//------Drawable2D
//------

std::vector<TriangleVertex> ImmutableMesh2D::getVertex() const {
    std::vector<TriangleVertex> v;

    for (auto &index: indices_) {
        TriangleVertex tri;
        //eigen vector没有实现begin，无法用ranged for
        tri.at(0) = vertex_pos_[index[0]];
        tri.at(1) = vertex_pos_[index[1]];
        tri.at(2) = vertex_pos_[index[2]];

        v.emplace_back(std::move(tri));
    }

    return v;
}

//---Rectangle
ConstRectangle::ConstRectangle() {
    //假设在x,y属于[-1,1],z=0的平面上
    const float left = -1.0;
    const float top = 1.0;
    const float right = 1.0;
    const float bottom = -1.0;
    const float z = 0.0;

    vertex_pos_.emplace_back(left,top,z);
    vertex_pos_.emplace_back(right,top,z);
    vertex_pos_.emplace_back(right,bottom,z);
    vertex_pos_.emplace_back(left,bottom,z);

    indices_.emplace_back(3,2,1);
    indices_.emplace_back(3,1,0);
}

Rectangle::Rectangle(float left, float top, float right, float bottom) {
    center_ = {(left+right)/2, (top+bottom)/2};
    scale_ = {std::abs(right-left), std::abs(top-bottom)};
}
Rectangle::Rectangle(RangeFloat left_right, RangeFloat top_bottom):
    Rectangle(left_right[0],top_bottom[0],left_right[1],top_bottom[1])
    {}

Rectangle::Rectangle(const Eigen::Vector2f& center, float width, float height):
    Mesh2D(center, {std::abs(width),std::abs(height)})
    {}

void Rectangle::getTriangles(std::vector<Triangle> &tri_list, OpOnVector3f &outer_op) const {

    auto opOnVertex = [this,&outer_op](const Vector3f &vertex) {
        return outer_op(this->simpleAffine2D(vertex));
    };

    const auto& single = ConstRectangle::instance();
    for (auto &vertex3: single.getVertex()) {
        Triangle tri;
        for (int j=0;j < 3;++j) {
            auto &&v = opOnVertex(vertex3[j]);
            tri.setVertex(j, v);
        }
        tri.setAllColors(TriangleMesh::mesh_color);

        tri_list.push_back(std::move(tri));
    }
}

//---Circle
template<unsigned int edges>
ConstCircleWEdges<edges>::ConstCircleWEdges() {
    //假设在x,y属于[-1,1],z=0的平面上
    const float radius = 1.0;
    const float z = 0.0;
    const float step_angle = 2.0 * M_PI / edges;
    Eigen::Array3f center(0,0,0);
    const int center_idx = edges;

    for (int i=0;i < edges;++i) {
        float angle = step_angle * i;
        Eigen::Array3f vert(std::cos(angle),std::sin(angle),z);

        vertex_pos_.emplace_back(vert * radius + center);
        indices_.emplace_back(center_idx,i,(i+1)%edges);
    }
    vertex_pos_.emplace_back(0,0,z);

}


Circle::Circle(const Eigen::Vector2f &center, float radius) :
    Mesh2D(center,{radius,radius})
    {}

void Circle::getTriangles(std::vector<Triangle> &tri_list, OpOnVector3f &outer_op) const {

    auto opOnVertex = [this,&outer_op](const Vector3f &vertex) {
        return outer_op(this->simpleAffine2D(vertex));
    };

    const auto& single = ConstCircle ::instance();
    for (auto &vertex3: single.getVertex()) {
        Triangle tri;
        for (int j=0;j < 3;++j) {
            auto &&v = opOnVertex(vertex3[j]);
            tri.setVertex(j, v);
        }
        tri.setAllColors(TriangleMesh::mesh_color);

        tri_list.push_back(std::move(tri));
    }
}

//Cone::Cone(const Eigen::Array3f &center, int edges, float radius, float tip_relative) {
//    assert (edges >= 3);
//#if GENERAL_DEBUG_MODE
//    if (std::abs(tip_relative) < 1e-7) {
//        throw std::invalid_argument("Cone received a zero *tip_relative* arg");
//    }
//#endif
//
//    axis_coef_ = tip_relative > 0 ? 1 : -1;
//    float step_angle = 2.0 * M_PI / edges;
//    Eigen::Array3f tip = Eigen::Array3f(0,tip_relative,0) * radius + center;
//    vertex_pos_.emplace_back(tip);
//
//    for (int i=0;i < edges;++i) {
//        float angle = step_angle * i;
//        Eigen::Array3f vert(std::cos(angle),0,std::sin(angle));
//        vertex_pos_.emplace_back(vert * radius + center);
//    }
//
//    if (SEE_VERTEX_AND_NORMAL) {
//        std::cout << "vertices\n";
//        for (auto v:vertex_pos_) {
//            std::cout << (v-Vector3f(0,0.5,0)).transpose().array()/0.5 << std::endl;
//        }
//        std::cout << "--------\n";
//
//    }
//
//    face_init(edges, false);
//}
//
//Cone::Cone(const voxel::HollowCircle &circle, float tip_relative, bool has_bottom) {
//    axis_coef_ = tip_relative > 0 ? 1 : -1;
//    Eigen::Array3f tip = Eigen::Array3f(0,tip_relative,0) * circle.radius_ + circle.center_;
//    vertex_pos_.emplace_back(tip);
//
//    vertex_pos_.insert(vertex_pos_.end(),
//                       circle.vertex_pos_.begin(),circle.vertex_pos_.end());
//
//    face_init(circle.edges_, has_bottom);
//}
//
//void Cone::face_init(int edges, bool has_bottom) {
//    //n-2个底面
//    Vector3f bottom_normal(0,-1,0);
//    vertex_normal_.emplace_back(bottom_normal * axis_coef_);
//    for (int i=2;i < edges;++i) {
//        indices_.emplace_back(1,i,i+1);
//        indices_vn_.emplace_back(Vector3i::Constant(0));
//    }
//    //n个侧面
//    for (int i=0;i < edges;++i) {
//        int idx_a = i, idx_b = i + 1, idx_vn = i + 1;
//        if (i == 0) idx_a = edges, idx_b = 1;
//
//        indices_.emplace_back(0,idx_a,idx_b);
//        Vector3f OA = vertex_pos_.at(idx_a) - vertex_pos_[0];
//        Vector3f OB = vertex_pos_.at(idx_b) - vertex_pos_[0];
//        const Vector3f &&normal = OB.cross(OA).normalized();
//        if (SEE_VERTEX_AND_NORMAL)
//            std::cout << normal.transpose() << std::endl;
//
//        vertex_normal_.emplace_back(normal * axis_coef_);
//        //由于编号0被底面占了，计数从1开始
//        assert(idx_vn >= 1);
//        indices_vn_.emplace_back(Vector3i::Constant(idx_vn));
//    }
//
//}
//
//Cube::Cube(float x1, float y1, float z1, float x2, float y2, float z2) {
//    //由于Cube对称性，这里的注释按照笛卡尔坐标而不是世界坐标
//    vertex_pos_.emplace_back(x1,y1,z1);
//    vertex_pos_.emplace_back(x1,y2,z1);
//    vertex_pos_.emplace_back(x2,y1,z1);
//    vertex_pos_.emplace_back(x2,y2,z1);
//    vertex_pos_.emplace_back(x1,y1,z2);
//    vertex_pos_.emplace_back(x1,y2,z2);
//    vertex_pos_.emplace_back(x2,y1,z2);
//    vertex_pos_.emplace_back(x2,y2,z2);
//
//    vertex_normal_.emplace_back(0,0,-1);
//    vertex_normal_.emplace_back(0,0, 1);
//    vertex_normal_.emplace_back(0,-1,0);
//    vertex_normal_.emplace_back(0, 1,0);
//    vertex_normal_.emplace_back(-1,0,0);
//    vertex_normal_.emplace_back( 1,0,0);
//
//    //bottom
//    indices_.emplace_back(1,2,0);
//    indices_.emplace_back(1,2,3);
//    indices_vn_.emplace_back(Vector3i::Constant(0));
//    indices_vn_.emplace_back(Vector3i::Constant(0));
//    //front
//    indices_.emplace_back(2,4,0);
//    indices_.emplace_back(2,4,6);
//    indices_vn_.emplace_back(Vector3i::Constant(2));
//    indices_vn_.emplace_back(Vector3i::Constant(2));
//    //left
//    indices_.emplace_back(1,4,0);
//    indices_.emplace_back(1,4,5);
//    indices_vn_.emplace_back(Vector3i::Constant(4));
//    indices_vn_.emplace_back(Vector3i::Constant(4));
//    //top
//    indices_.emplace_back(5,6,4);
//    indices_.emplace_back(5,6,7);
//    indices_vn_.emplace_back(Vector3i::Constant(1));
//    indices_vn_.emplace_back(Vector3i::Constant(1));
//    //back
//    indices_.emplace_back(3,5,1);
//    indices_.emplace_back(3,5,7);
//    indices_vn_.emplace_back(Vector3i::Constant(3));
//    indices_vn_.emplace_back(Vector3i::Constant(3));
//    //right
//    indices_.emplace_back(3,6,2);
//    indices_.emplace_back(3,6,7);
//    indices_vn_.emplace_back(Vector3i::Constant(5));
//    indices_vn_.emplace_back(Vector3i::Constant(5));
//}
//
//Frustum::Frustum(const HollowCircle &lower, const HollowCircle &upper) {
//    if (lower.edges_ != upper.edges_) {
//        throw std::invalid_argument("2 circles' edges are different");
//    }
//
//    vertex_pos_.insert(vertex_pos_.begin(),
//                       lower.vertex_pos_.begin(),lower.vertex_pos_.end());
//
////    Eigen::AngleAxis<float> rot(-M_PI/upper.edges_, Vector3f({0,1,0}));
////    auto &&rotate = rot.toRotationMatrix();
//    for (const auto &v : upper.vertex_pos_) {
//        vertex_pos_.emplace_back(v);
//    }
//
//    int n = lower.edges_;
//    for (int i = 0; i < n; ++i) {
//        //上下三角形穿插
//        //插入顶点顺序按右手螺旋，方便计算normal
//        indices_.emplace_back(i, (i+1)%n, n+i);
//        indices_.emplace_back((i+1)%n, n+(i+1)%n, n+i);
//    }
//
//    float axis_coef_=1;
//    //2n个侧面法向
//    for (int i=0;i < 2*n;++i) {
//        Eigen::Array3i idx = indices_.at(i);
//        int idx_a = idx[0], idx_b = idx[1], idx_c = idx[2];
//
//        Vector3f OA = vertex_pos_.at(idx_b) - vertex_pos_.at(idx_a);
//        Vector3f OB = vertex_pos_.at(idx_c) - vertex_pos_.at(idx_a);
//        const Vector3f &&normal = OB.cross(OA).normalized();
//
//        vertex_normal_.emplace_back(normal * axis_coef_);
//        indices_vn_.emplace_back(Vector3i::Constant(i));
//    }
//}
//
//Sphere::Sphere(const Eigen::Array3f &center, const float radius, int edges, int parallel) {
//    assert(edges >= 3);
//    assert(parallel > 0);
//
//    //准备构造纬度圈
//    std::vector<HollowCircle> circles;
//    circles.reserve(2*parallel);
//    Eigen::Array3f bias(0,0,radius);
//    auto cal_sin = [radius] (const Eigen::Array3f &bias) {
//        float cos = bias.z();
//        return std::sqrt(radius*radius - cos*cos);
//    };
//    //构造纬度圈
//    double angle=0;
//    for (int i = 1-parallel; i < parallel; ++i) {
//        Eigen::Array3f c_bias = (bias / parallel * i);
//        HollowCircle circle(c_bias + center, cal_sin(c_bias), edges);
//
//        //旋转圈，使其能形成三角形交错而不是矩形
//        Eigen::AngleAxis<float> rot(-angle, Vector3f({0,1,0}));
//        auto &&rotate = rot.toRotationMatrix();
//        auto rotate_fn = [&rotate](auto& x) {x = rotate*x;};
//        std::for_each(circle.vertex_pos_.begin(),circle.vertex_pos_.end(),rotate_fn);
//        std::for_each(circle.vertex_normal_.begin(),circle.vertex_normal_.end(),rotate_fn);
//
//        circles.emplace_back(std::move(circle));
//        angle += M_PI/edges;
//    }
//
//    //底部顶部圆锥
//    float last_cos = double(radius) / parallel;
//    float relative_tip = last_cos / circles.at(0).radius_;
//    Cone cone_bot(circles.at(0),-relative_tip),
//        cone_top(circles.at(circles.size()-1),relative_tip);
//
//    auto copy_from_other = [this](Mesh3D &ano) {
//        auto num_pos = Eigen::Vector3i::Constant(this->vertex_pos_.size());
//        this->vertex_pos_.insert(this->vertex_pos_.end(),
//                                 ano.vertex_pos_.begin(), ano.vertex_pos_.end());
//
//        for (Vector3i &idx : ano.indices_) {
//            this->indices_.emplace_back(idx+num_pos);
//        }
//
//        auto num_vn = Eigen::Vector3i::Constant(this->vertex_normal_.size());
//        this->vertex_normal_.insert(this->vertex_normal_.end(),
//                                 ano.vertex_normal_.begin(), ano.vertex_normal_.end());
//        for (Vector3i &idx : ano.indices_vn_) {
//            this->indices_vn_.emplace_back(idx+num_vn);
//        }
//    };
//
//    copy_from_other(cone_bot);
//    for (int i=1;i < circles.size();++i) {
//        Frustum frustum(circles.at(i-1),circles.at(i));
//        copy_from_other(frustum);
//    }
//    copy_from_other(cone_top);
//}
}