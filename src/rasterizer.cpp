//
// Created on 2022/5/27.
//

#include "rasterizer.hpp"

#include <cmath>
#include <climits>
#include <array>

#include "drawing_methods.hpp"
#include "utils.hpp"

typedef const Eigen::Vector3f& Vector3fCRef;
typedef std::tuple<float, float, float> Tuple3Df;

struct InterpolateFn {
    //alpha -> AB, beta -> AC
    float alpha, beta, gamma;
    InterpolateFn(float a, float b, float c): alpha(a), beta(b), gamma(c) {}
    InterpolateFn(): InterpolateFn(0,0,0) {}

    template<typename T>
    inline T operator() (T a, T b, T c) {
        return alpha * b + beta * c + gamma * a;
    }
};

Rasterizer::Rasterizer(int h, int w): width_(w),height_(h) {
    frame_buffer_.resize({height_, width_, 3});
    z_buffer_.resize(height_, width_);
    clearBuffer(Buffers::Depth | Buffers::Color);

    model_ = view_ = projection_ = Eigen::Matrix4f::Identity();
}

void Rasterizer::clearBuffer(Buffers buffer_instruct) {
    //为了方便只能用Implicit，是否会有其他问题？
    int buf=buffer_instruct;
    if ((buf & Buffers::Color) == Buffers::Color) {
        frame_buffer_.setConstant(0);
    }
    if ((buf & Buffers::Depth) == Buffers::Depth) {
        z_buffer_.setConstant(std::numeric_limits<ZBufferType>::infinity());
    }
}

using namespace line_drawing;
void Rasterizer::drawLine(Vector3f begin,Vector3f end) {
    drawLine_Bresenham(begin.x(),begin.y(),end.x(),end.y(),*this);

}

Tuple3Df computeBarycentric2D(Vector3fCRef P, Vector3fCRef A, Vector3fCRef B, Vector3fCRef C) {
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

    return {cross.x(), cross.z(), -cross.y()};
}

inline auto computeBarycentric2D(const Vector3f &P, const array<Vector3f ,3> &vertex) {
    return computeBarycentric2D(P, vertex[0], vertex[1], vertex[2]);
}

inline bool insideTriangle_Barycentric(float a,float b,float c) {
    return a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0 && c <= 1;
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
    Eigen::Matrix3f vertex_data;
    vertex_data << tri.vertex_[0], tri.vertex_[1], tri.vertex_[2];

    Vector3f &&min_v = vertex_data.rowwise().minCoeff();
    int infX = std::floor(min_v.x()), infY = std::floor(min_v.y());
    Vector3f &&max_v = vertex_data.rowwise().maxCoeff();
    int supX = std::floor(max_v.x()), supY = std::floor(max_v.y());
#if DEBUG_MODE > 1
    std::clog << "drawTriangle received:\n" << vertex_data << std::endl;
    std::clog << "boundings: " << infX << ' ' << infY << ' ' << supX << ' ' << supY << std::endl;
#endif

    for (int x = infX;x < supX;++x) {
        for (int y = infY;y < supY;++y) {
            //若使用C++17结构化绑定，clang会在下面的lambda表达式报错，因此用C++11版本
            if (!insideTriangle(x,y,tri.vertex_)) {
                continue;
            }


            float alpha, beta, gamma, det;
            std::tie(alpha, beta, det) = computeBarycentric2D(Vector3f(x,y,0.0), tri.vertex_);
            if (det == 0) {
                std::cerr << "drawing a line-shape triangle.\n";
                return;
            }
            alpha /= det, beta /= det;
            gamma = 1 - alpha - beta;
//            if (!insideTriangle_Barycentric(alpha,beta,gamma)) {
//                continue;
//            }

            InterpolateFn interpolate(alpha,beta,gamma);


            ZBufferType cur_depth = getZBuffer(x,y);
            float z = interpolate(vertex_data(2,0), vertex_data(2,1), vertex_data(2,2));
            if (z > cur_depth)
                continue;

            setDepth(x,y,z);

            ColorType color = interpolate(tri.getColor(0),tri.getColor(1),tri.getColor(2));
            setPixel(x,y,color);
        }
    }

}

void Rasterizer::draw(const std::vector<Triangle> &triangles) {

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;


    Eigen::Matrix4f mv  = view_ * model_;
    Eigen::Matrix4f mvp = projection_ * view_ * model_;
    std::cout << mvp << std::endl;

    for (const auto& tri : triangles) {
        //之后换成指针
        Triangle new_tri = tri;

        std::array<Eigen::Vector4f, 3> vertex;
        for (int i=0;i < 3;++i) {
            vertex[i] = Vector3to4(tri.vertex_[i]);
        }

        std::array<Vector3f, 3> viewspace_pos;
        for (int i=0;i < 3;++i) {
            viewspace_pos[i] = (mv * vertex[i]).head(3);
        }
//        std::cout << tri.vertex_[0] << std::endl;
//        std::cout << viewspace_pos[0] << std::endl;

        for (int i=0;i < 3;++i) {
            Eigen::Vector4f new_vertex = mvp * vertex[i];
            new_vertex/=new_vertex.w();
//            std::cout << "point " << new_vertex << std::endl;
            new_vertex.x() = 0.5 * width_ * (1.0f + new_vertex.x());
            new_vertex.y() = 0.5 * height_ * (1.0f + new_vertex.y());
            new_vertex.z() = new_vertex.z() * f1 + f2;

            new_tri.vertex_[i] = new_vertex.head(3);
        }

        //TODO: 设置normal

        drawTriangle(new_tri);
    }

}
