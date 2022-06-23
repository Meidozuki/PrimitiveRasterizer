//
// Created on 2022/5/27.
//

#ifndef MAIN_CPP_RASTERIZER_HPP
#define MAIN_CPP_RASTERIZER_HPP

#include <iostream>
#include <tuple>
#include <map>

#include <Eigen/Core>
#include "unsupported/Eigen/CXX11/Tensor"

#include "util_define.hpp"
#include "triangle.hpp"
#include "shader.hpp"

#ifndef GENERAL_DEBUG_MODE
  #define GENERAL_DEBUG_MODE 1
#endif

using DType = float;
using Eigen::Tensor;
using Eigen::Dynamic;
using Eigen::Vector3f;
using std::stringstream;
using std::array;

struct Buffers {
    enum Buffer : int{
        Color = 1,
        Depth = 2,
    };

    int buffer_;
    explicit Buffers(Buffer buf):buffer_(buf) {}
    Buffers(int buf):buffer_(buf) {}
    inline int asInt() const {return static_cast<int>(buffer_);}
    explicit operator int() const {return static_cast<int>(buffer_);}
};

class Rasterizer {
public:
    enum class DrawingMode {Wireframe, Triangle};

    using PositionIndex = int;
    using IndicesIndex = int;
    using ColorIndex = int;

    using ZBufferType = float;

private:
    Tensor<DType, 3> frame_buffer_; // 3-dim tensor
    Eigen::Matrix<ZBufferType, Dynamic, Dynamic> z_buffer_;

    int width_, height_;
    Eigen::Matrix4f model_, view_, projection_;
    Eigen::Vector3f eye_pos_;

    Shader shader_;

public:
    Rasterizer(int h,int w);
    Rasterizer(): Rasterizer(0,0) {};

    //getters
    int width() const {return width_;}
    int height() const {return height_;}
    const auto & framebuffer() {return frame_buffer_;}
    float getZBuffer(int x,int y) {return z_buffer_(y,x);}
    std::tuple<int, int> get_framebuffer_shape() {
        return {frame_buffer_.dimension(0),frame_buffer_.dimension(1)};
    }
    Eigen::Vector3f getShadeEyePos();

    //setters
    void clearBuffer(Buffers );
    void setEyePos(const Eigen::Vector3f &pos);

    void setModel(const Eigen::Matrix4f &model)         {model_ = model;}
    void setView(const Eigen::Matrix4f &view)           {view_ = view;}
    void setProjection(const Eigen::Matrix4f &project)  {projection_ = project;}
    inline void setPixel(int x,int y,const ColorType& color);
    inline void setDepth(int x,int y,ZBufferType z);

    void draw(const std::vector<Triangle> &triangles);
    void drawLine(const Vector3f & begin, const Vector3f & end, const ColorType &color);
    void drawTriangle(const Triangle &tri, const array<Vector3f, 3> &shade_point);

};


inline void Rasterizer::setPixel(int x,int y,const ColorType& color) {
#if GENERAL_DEBUG_MODE
    if (x < 0 || x > width_ ||
        y < 0 || y > height_)
    {
        stringstream err_info;
        if (x < 0 || x > width_) {
            err_info << "setPixel: x out of range, x = " << x;
        }
        else {
            err_info << "setPixel: y out of range, y = " << y;
        }
        throw std::out_of_range(err_info.str());
    }
#endif

    frame_buffer_(y, x, 0) = color[0];
    frame_buffer_(y, x, 1) = color[1];
    frame_buffer_(y, x, 2) = color[2];
}

inline void Rasterizer::setDepth(int x, int y, ZBufferType z) {
#if GENERAL_DEBUG_MODE
    if (x < 0 || x > width_ ||
        y < 0 || y > height_)
    {
        stringstream err_info;
        if (x < 0 || x > width_) {
            err_info << "setDepth: x out of range, x = " << x;
        }
        else {
            err_info << "setDepth: y out of range, y = " << y;
        }
        throw std::out_of_range(err_info.str());
    }
#endif

    z_buffer_(y, x) = z;
}

#endif //MAIN_CPP_RASTERIZER_HPP
