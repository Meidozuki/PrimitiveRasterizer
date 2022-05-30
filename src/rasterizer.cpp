//
// Created on 2022/5/27.
//

#include "rasterizer.hpp"

#include <cmath>
#include <array>

#include "drawing_methods.hpp"

typedef const Eigen::Vector3f& Vector3fCRef;
typedef std::tuple<float, float, float, float> Tuple4Df;

using namespace line_drawing;
void Rasterizer::drawLine(Vector3f begin,Vector3f end) {
    drawLine_Bresenham(begin.x(),begin.y(),end.x(),end.y(),*this);

}

Tuple4Df computeBarycentric2D(Vector3fCRef P, Vector3fCRef A, Vector3fCRef B, Vector3fCRef C) {
    //忽略z轴
    //求解alpha * AB + beta * AC = AP
    //用Cramer法则求解，可以转化为叉乘

    //此处实际只要Vector2f，但用Vector3f避免转化
    Eigen::Vector3f &&AP = P - A;
    Eigen::Vector3f &&AB = B - A;
    Eigen::Vector3f &&AC = C - A;

    Eigen::Vector3f X (AB.x(), AP.x(), AC.x());
    Eigen::Vector3f Y (AB.y(), AP.y(), AC.y());
    //这个形式的叉乘刚好返回(alpha, -Det, beta)
    Eigen::Vector3f && cross=X.cross(Y);

    return {cross.x(), cross.z(), 1-cross.x()-cross.z(), -cross.y()};
}

inline auto computeBarycentric2D(const Vector3f &P, const array<Vector3f ,3> &vertex) {
    return computeBarycentric2D(P, vertex[0], vertex[1], vertex[2]);
}

bool insideTriangle(int x, int y, const std::array<Vector3f, 3> &vertices) {
    //由于在二维平面上，点(x,y)一定宇三角形共面
    //那么(x,y)一定能插值出唯一确定的z，于是可以丢弃z轴信息

    //ps: Eigen计算叉乘只支持三维
    //将所有点挪到xoy平面
    Vector3f P(x,y,0.0);
    Vector3f A(vertices[0].x(), vertices[0].y(), 0.0);
    Vector3f B(vertices[1].x(), vertices[1].y(), 0.0);
    Vector3f C(vertices[2].x(), vertices[2].y(), 0.0);

    //计算叉乘来判断是否在三角形内
    float z1 = (P - A).cross(B - A) .z();
    float z2 = (P - B).cross(C - B) .z();
    float z3 = (P - C).cross(A - C) .z();

    return (z1 > 0 && z2 > 0 && z3 > 0) || (z1 < 0 && z2 < 0 && z3 < 0);
}

void Rasterizer::drawTriangle(const Triangle &tri) {
    //TODO: 处理edge
    ColorType color{1,1,1};
    Eigen::Matrix3f vertex_data;
    vertex_data << tri.vertex_[0], tri.vertex_[1], tri.vertex_[2];

#if DEBUG_MODE
    std::clog << "drawTriangle received:\n" << vertex_data << std::endl;
#endif

    Vector3f &&min_v = vertex_data.rowwise().minCoeff();
    int infX = std::floor(min_v.x()), infY = std::floor(min_v.y());
    Vector3f &&max_v = vertex_data.rowwise().maxCoeff();
    int supX = std::floor(max_v.x()), supY = std::floor(max_v.y());
//    std::cout << infX << ' ' << infY << ' ' << supX << ' ' << supY << std::endl;

    for (int i = infX;i < supX;++i) {
        for (int j = infY;j < supY;++j) {
            if (!insideTriangle(i,j,tri.vertex_)) {
                continue;
            }

            Vector3f P(static_cast<float>(i), static_cast<float>(j), 0.0f);
            auto [alpha, beta, gamma, det] = computeBarycentric2D(P, tri.vertex_);

            int depth=99,z=1;
            if (z > depth) continue;

            setPixel(i,j,color);
        }
    }
}