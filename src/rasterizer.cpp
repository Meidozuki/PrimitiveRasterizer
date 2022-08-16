//
// Created on 2022/5/27.
//

#include "rasterizer.hpp"

#include <cmath>
#include <climits>
#include <array>

#include "straight_line.hpp"
#include "basic_matrix.hpp"
#include "image_io.hpp"

using Tuple3Df = std::tuple<float, float, float>;
using std::cout, std::endl;

//返回一个lambda表达式
//比起全局变量，这种方法会多一步lea，但是由于参数不同，不使用全局
//为了能兼容多线程，也不适用全局变量更改参数的方法
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

//TODO:去看下汇编这两种是否有性能差异
//直接定义一个函数类实例
static struct {
    template <typename T>
    using Vector_t = Eigen::Matrix<T,3,1> ;

    template <typename T>
    std::tuple<Vector_t<T>,Vector_t<T> > operator() (const std::array<Vector_t<T>, 3> &vertices) {
        Eigen::Matrix<T, 3, 3> vertex_data;
        vertex_data << vertices[0], vertices[1], vertices[2];

        Vector_t<T> &&min_v = vertex_data.rowwise().minCoeff();
        Vector_t<T> &&max_v = vertex_data.rowwise().maxCoeff();
        return {min_v, max_v};
    }
} findBoundary;


//————————Constructor
Rasterizer::Rasterizer(int h, int w): width_(w),height_(h) {
    frame_buffer_.resize({height_, width_, 3});
    z_buffer_.resize(height_, width_);
    clearBuffer(Buffers::Depth | Buffers::Color);

    model_ = view_ = projection_ = Eigen::Matrix4f::Identity();
}


//————————setters
void Rasterizer::clearBuffer(Buffers buffer_instruct) {
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
    vertex_data << tri.vertex(0), tri.vertex(1), tri.vertex(2);
//
//    Vector3f &&min_v = vertex_data.rowwise().minCoeff();
//    int infX = std::floor(min_v.x()), infY = std::floor(min_v.y());
//    Vector3f &&max_v = vertex_data.rowwise().maxCoeff();
//    int supX = std::floor(max_v.x()), supY = std::floor(max_v.y());
    auto [min_v,max_v] = findBoundary(tri.getVertices());
    int infX = std::floor(min_v.x()),
        infY = std::floor(min_v.y());
    int supX = std::ceil(max_v.x()),
        supY = std::ceil(max_v.y());

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
            //若使用C++17结构化绑定，clang会在下面的lambda表达式报warning，因此用C++11版本
            float alpha, beta, gamma, det;
            std::tie(alpha, beta, det) = computeBarycentric2D(Vector3f(x,y,0.0), tri.getVertices());
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

            ColorType interpolated_color = interpolate(tri.color(0), tri.color(1), tri.color(2));


            //世界坐标系下的xyz
            Vector3f interpolated_pos = interpolate(shade_point[0], shade_point[1], shade_point[2]);
            Vector3f normal = interpolate(tri.normal(0),tri.normal(1),tri.normal(2));


            auto new_color = shader_.shadeColor(interpolated_pos, normal, interpolated_color);
//            std::cout << "color " << interpolated_color.transpose() << " after " << new_color.transpose() << std::endl;

            setPixel(x,y,new_color);
        } //endfor y
    } //endfor x

}


#include <sstream>
void Rasterizer::draw(const std::vector<Triangle> &triangles) {

    float z_scale = (50 - 0.1) / 2.0, z_affine = (50 + 0.1) / 2.0;

    shader_.eye_pos_ = getShadeEyePos();

    Eigen::Matrix4f mvp = projection_ * view_ * model_;
//    std::cout << "mvp" << mvp << std::endl;

//    std::vector<Triangle> regular_tri;

    for (const auto& tri : triangles) {
        Triangle new_tri = tri;

        //设置顶点
        std::array<Eigen::Vector4f, 3> vertex;

        //转换到投影坐标系下
        for (int i=0;i < 3;++i) {
            vertex[i] = Vector3to4(tri.vertex(i));

            Eigen::Vector4f new_vertex = mvp * vertex[i];
            new_vertex /= new_vertex.w();

            new_tri.setVertex(i, new_vertex.head(3));
        }

        //假设已经规范化，裁剪
        auto [min_v,max_v] = findBoundary(new_tri.getVertices());
        float xmin = min_v.x(), ymin =min_v.y();
        float xmax = max_v.x(), ymax =max_v.y();

        //简单clip
        if (xmin > 1.0 || xmax < -1.0 || ymin > 1.0 || ymax < -1.0) {
            continue;
        }

        //在世界坐标系下shading
        static int cnt=0;
        for (int i=0;i < 3;++i) {
            new_tri.setNormal(i, tri.normal(i));
        }

        for (int i=0;i < 3;++i) {
            //视口变换
            auto vert = new_tri.vertex(i);
            vert.x() = 0.5 * width_ * (1.0f + vert.x());
            vert.y() = 0.5 * height_ * (1.0f + vert.y());
            vert.z() = vert.z() * z_scale + z_affine;
            new_tri.setVertex(i,vert);
        }

//        regular_tri.emplace_back(std::move(new_tri));


        //需要更改shading坐标系才能按照标准pipeline
        drawTriangle(new_tri, tri.getVertices());


    }

}
