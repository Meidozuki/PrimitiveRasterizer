//
// Created on 2022/5/27.
//

#include "rasterizer.hpp"

#include <cmath>
#include <climits>
#include <array>

#include "util_define.hpp"
#include "util_func.hpp"
#include "straight_line.hpp"
#include "basic_matrix.hpp"

typedef std::tuple<float, float, float> Tuple3Df;
using std::cout, std::endl;

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


//————————Constructor
Rasterizer::Rasterizer(int h, int w): width_(w),height_(h) {
    frame_buffer_.resize({height_, width_, 3});
    z_buffer_.resize(height_, width_);
    clearBuffer(Buffers::Depth | Buffers::Color);

    model_ = view_ = projection_ = Eigen::Matrix4f::Identity();
}


//————————setters
void Rasterizer::clearBuffer(Buffers buffer_instruct) {
    //改-为了方便只能用Implicit，是否会有其他问题？
    int buf=buffer_instruct.asInt();
    //编译器将enum转int
    if ((buf & Buffers::Color) == Buffers::Color) {
        frame_buffer_.setConstant(0);
    }
    if ((buf & Buffers::Depth) == Buffers::Depth) {
        z_buffer_.setConstant(std::numeric_limits<ZBufferType>::infinity());
    }
}

void Rasterizer::setEyePos(const Eigen::Vector3f &pos) {
    shader_.eye_pos_ = pos;
    eye_pos_ = pos;
    setView(getViewMatrix(eye_pos_));
}


//————————getters
Eigen::Vector3f Rasterizer::getShadeEyePos() {
    //TODO:绑定变化，减少计算
    static Eigen::Matrix4f inv = model_.inverse(), previous_model = model_;
    if (model_ != previous_model) {
        inv = model_.inverse();
        previous_model = model_;
    }
    Eigen::Vector4f &&pos = inv * Vector3to4(eye_pos_);
    return (pos / pos.w()).head(3);
}

//————————drawing
void Rasterizer::drawLine(const Vector3f & begin, const Vector3f & end, const ColorType &color) {
    Line line;
    line.begin = begin;
    line.end = end;
    line.color = color;
    line_drawing::drawLine(line,*this);

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


void Rasterizer::drawTriangle(const Triangle &tri, const array<Vector3f, 3> &shade_point) {
    //TODO: 处理edge情况
    Eigen::Matrix3f vertex_data;
    vertex_data << tri.vertex_[0], tri.vertex_[1], tri.vertex_[2];

    Vector3f &&min_v = vertex_data.rowwise().minCoeff();
    int infX = std::floor(min_v.x()), infY = std::floor(min_v.y());
    Vector3f &&max_v = vertex_data.rowwise().maxCoeff();
    int supX = std::floor(max_v.x()), supY = std::floor(max_v.y());
#if GENERAL_DEBUG_MODE
    if (infX < 0 || infY < 0 || supX >= width_ || supY >= height_) {
        std::cerr << "drawTriangle: X or Y out of range,consider clip" << std::endl;
        std::cerr << "infX,infY,supX,supY = " << infX << ',' << infY<< ',' << supX << ',' << supY << std::endl;
        infX = std::max(0,infX), infY = std::max(0,infY);
        supX = std::min(width_,supX), supY = std::min(height_,supY);
    }
#endif
#if VERBOSE_DEBUG_MODE > 1
    std::clog << "drawTriangle received:\n" << vertex_data << std::endl;
    std::clog << "boundings: " << infX << ' ' << infY << ' ' << supX << ' ' << supY << std::endl;
#endif

    for (int x = infX;x < supX;++x) {
        for (int y = infY;y < supY;++y) {
            //若使用C++17结构化绑定，clang会在下面的lambda表达式报错，因此用C++11版本
            float alpha, beta, gamma, det;
            std::tie(alpha, beta, det) = computeBarycentric2D(Vector3f(x,y,0.0), tri.vertex_);
            if (std::abs(det) <= 1e-5) {
                std::cerr << "drawing a line-shape triangle.\n";
                return;
            }
            alpha /= det, beta /= det;
            gamma = 1 - alpha - beta;
            if (!insideTriangle_Barycentric(alpha,beta,gamma)) {
                continue;
            }

            InterpolateFn interpolate(alpha,beta,gamma);


            ZBufferType cur_depth = getZBuffer(x,y);
            float z = interpolate(vertex_data(2,0), vertex_data(2,1), vertex_data(2,2));
            if (z > cur_depth)
                continue;

            setDepth(x,y,z);

            ColorType interpolated_color = interpolate(tri.getColor(0),tri.getColor(1),tri.getColor(2));


            //世界坐标系下的xyz
            Vector3f interpolated_pos = interpolate(shade_point[0], shade_point[1], shade_point[2]);
            Vector3f normal = interpolate(tri.normal_[0],tri.normal_[1],tri.normal_[2]);


            auto new_color = shader_.shadeColor(interpolated_pos, normal, interpolated_color);
//            std::cout << "color " << interpolated_color.transpose() << " after " << new_color.transpose() << std::endl;

            setPixel(x,y,new_color);
        }
    }

}

void Rasterizer::draw(const std::vector<Triangle> &triangles) {


    float z_scale = (50 - 0.1) / 2.0, z_affine = (50 + 0.1) / 2.0;

    shader_.eye_pos_ = getShadeEyePos();

    Eigen::Matrix4f mv  = view_ * model_;
    Eigen::Matrix4f mvp = projection_ * view_ * model_;
//    std::cout << "mvp" << mvp << std::endl;

    for (const auto& tri : triangles) {
        //之后换成指针
        Triangle new_tri = tri;

        //设置顶点
        std::array<Eigen::Vector4f, 3> vertex;

        for (int i=0;i < 3;++i) {
            vertex[i] = Vector3to4(tri.vertex_[i]);

            Eigen::Vector4f new_vertex = mvp * vertex[i];
            new_vertex /= new_vertex.w();
//            std::cout << "point " << new_vertex << std::endl;
            new_vertex.x() = 0.5 * width_ * (1.0f + new_vertex.x());
            new_vertex.y() = 0.5 * height_ * (1.0f + new_vertex.y());
            new_vertex.z() = new_vertex.z() * z_scale + z_affine;

            new_tri.vertex_[i] = new_vertex.head(3);
        }

        //寻找shading point
//        std::array<Vector3f, 3> shade_point;
//        for (int i=0;i < 3;++i) {
//            shade_point[i] = (mv * vertex[i]).head(3);
//        }
//        std::cout << tri.vertex_[0] << std::endl;
//        std::cout << shade_point[0] << std::endl;

        //设置normal
//        Eigen::Matrix4f inv_trans = mv.reverse().transpose();
//        Eigen::Vector4f n[3];
        for (int i=0;i < 3;++i) {
//            n[i] = inv_trans * Vector3to4(tri.normal_[i],0);
//            new_tri.normal_[i] = n[i].head(3);
            new_tri.normal_[i] = tri.normal_[i];
        }


        drawTriangle(new_tri, tri.vertex_);
    }

}
