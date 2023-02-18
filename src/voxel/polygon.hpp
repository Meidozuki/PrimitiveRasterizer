//
// Created by user on 2022/8/4.
//

#ifndef RASTERIZER_POLYGON_HPP
#define RASTERIZER_POLYGON_HPP

#include <vector>
#include <array>
#include <functional>

#include <Eigen/Core>

#include "../FP_util.hpp"
#include "../Singleton.hpp"
#include "util_define.hpp"
#include "triangle.hpp"

using RangeFloat = Eigen::Vector2f;

using Eigen::Vector3f;
using Eigen::Vector3i;

using OpOnVector3f = std::function<Vector3f(const Vector3f&)>;
using TriangleVertex = std::array<Vector3f, 3>;

inline auto identityFn = OpOnVector3f([](const Vector3f& x) noexcept {return x;}) ;

namespace voxel {

Trait TriangleMesh {
    static ColorType mesh_color;
    static void setMeshColor(const Vector3f &color) {mesh_color = color;}

    std::vector<Eigen::Vector3f> vertex_pos_;
    std::vector<Eigen::Vector3i> indices_;

    ABSTRACT(TriangleMesh);
};

Trait TriangleMesh3D: public TriangleMesh {
    std::vector<Eigen::Vector3f> vertex_normal_;
    std::vector<Eigen::Vector3i> indices_vn_;

    ABSTRACT(TriangleMesh3D);
};

//static member
//需要C++17
inline ColorType TriangleMesh::mesh_color = Vector3f(1, 1, 1);

struct DrawableBase{
protected:
    DrawableBase() = default;
};

struct Drawable2D: DrawableBase {};

struct Drawable3D: DrawableBase {};

//Mesh2D Singletons，Mesh2D在其上用仿射矩阵变换，减少不必要的顶点存储
//假设在x,y属于[-1,1],z=0的平面上
class ImmutableMesh2D: protected TriangleMesh {
protected:
    std::vector<TriangleVertex> getVertex() const;
    ABSTRACT(ImmutableMesh2D);
};


//---Mesh2D
class Mesh2D : Drawable2D {
protected:
    Eigen::Array2f center_;
    Eigen::Array2f scale_;

    Mesh2D() = default;
    Mesh2D(Eigen::Array2f center, Eigen::Array2f scale):
        center_(std::move(center)),scale_(std::move(scale)) {}

    Vector3f simpleAffine2D(const Vector3f& vertex) const {
        Eigen::Vector3f matrix_scale(scale_.x(), scale_.y(), 1.0);
        Eigen::Vector3f matrix_center(center_.x(), center_.y(), 0.0);
        return matrix_center + matrix_scale.cwiseProduct(vertex);
    }

public:
    void getTriangles(std::vector<Triangle> &tri_list) const {
        return getTriangles(tri_list, identityFn);
    }

    virtual void getTriangles(std::vector<Triangle> &tri_list, OpOnVector3f &outer_op) const = 0;
};


class Rectangle: public Mesh2D {
public:
    Rectangle(): Rectangle({0,0},2,2) {}
    Rectangle(RangeFloat left_right, RangeFloat top_bottom);
    Rectangle(const Eigen::Vector2f& center, float width, float height);

    void getTriangles(std::vector<Triangle> &tri_list, OpOnVector3f &outer_op) const override;
protected:
    //参考系为屏幕
    Rectangle(float left, float top, float right, float bottom);
};

class ConstRectangle final: public ImmutableMesh2D, protected Singleton<ConstRectangle> {
public:
    friend class Singleton<SingletonType>;
    friend class Rectangle;

    [[gnu::const]]
    static const SingletonType& instance() {return Singleton<SingletonType>::getInstance();}
protected:
    ConstRectangle();
};



template <unsigned int edges>
class ConstCircleWEdges;


class Circle : public Mesh2D {
private:
    constexpr static int edges = 32;
    using ConstCircle = ConstCircleWEdges<edges>;
public:
    Circle(const Eigen::Vector2f &center, float radius);

    void getTriangles(std::vector<Triangle> &tri_list, OpOnVector3f &outer_op) const override;
};

template <unsigned int edges>
class ConstCircleWEdges final: public ImmutableMesh2D, protected Singleton<ConstCircleWEdges<edges>> {
    //模板类为dependent name，与上面的普通类不同，需要显示指定
    using SingletonType = typename Singleton<ConstCircleWEdges<edges>>::SingletonType ;
public:
    friend class Singleton<SingletonType>;
    friend class Circle;

    [[gnu::const]]
    static const SingletonType& instance() {return Singleton<SingletonType>::getInstance();}

protected:
    ConstCircleWEdges();
};

class DynamicCircle: public Circle {
private:
    int edges_;
public:
    DynamicCircle(const Eigen::Vector2f &center, float radius, unsigned int edges):
        Circle(center,radius),edges_(edges)
        {}

    void getTriangles(std::vector<Triangle> &tri_list, OpOnVector3f &outer_op) const override;
};


/**
 * @brief HollowMesh意为绘制Mesh3D时提供顶点等快捷方法，最终运行性能视情况而定
 *      HollowMesh同时为一层wrapper，提供面向3D的接口
 */
Trait HollowMesh {};

class HollowCircle : protected Circle,public HollowMesh {
//public:
//    Eigen::Array3f center_;
//    float radius_;
//    unsigned int edges_;
//
//    HollowCircle() = default;
//    HollowCircle(Vector3f center,float radius):
//        Circle<edges>({center.x(),center.y()}, radius), radius_(radius), edges_(edges) {
////        center_={center.x(),center.z(),center.y()};
//    }
};

class Plane : public Drawable3D {

    enum AlignedAxis {axisX,axisY,axisZ};
    AlignedAxis aligned_axis;
    float z_;

//    Mesh2D(): z_(1.0) {aligned_axis = axisZ;}

    void setAlignedAxis(AlignedAxis axis) {aligned_axis = axis;}
    void getTriangles(std::vector<Triangle> &tri_list);
};


//---Mesh3D
class Mesh3D : public Drawable3D, protected TriangleMesh3D{
public:
    using TriangleMesh3D::vertex_pos_, TriangleMesh3D::indices_,
        TriangleMesh3D::vertex_normal_, TriangleMesh3D::indices_vn_;
};

class Cube : public Mesh3D {
public:
    Cube();
    Cube(float x1,float y1,float z1,float x2,float y2,float z2);
};
using Cuboid = Cube; //长方体

class Cone : public Mesh3D {
public:
    //默认正多棱锥
    Cone();
    Cone(const Eigen::Array3f &center, int edges, float radius = 1.0, float tip_relative = 1.0);
    Cone(const HollowCircle &circle, float tip_relative = 1.0, bool has_bottom = true);

private:
    float axis_coef_;
    void face_init(int edges, bool has_bottom = true);
};

class Frustum : public Mesh3D {
public:
    Frustum();
    Frustum(const HollowCircle &lower,const HollowCircle &upper);
};

class Sphere : public Mesh3D {
public:
    Sphere();
    Sphere(const Eigen::Array3f &center, float radius, int edges=64, int parallel=20);
};

}

#endif //RASTERIZER_POLYGON_HPP
