//
// Created on 2022/5/27.
//

#ifndef MAIN_CPP_RASTERIZER_HPP
#define MAIN_CPP_RASTERIZER_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <map>

#include <Eigen/Core>
#include "unsupported/Eigen/CXX11/Tensor"

#include "utils.hpp"
#include "triangle.hpp"

#ifndef DEBUG_MODE
  #define DEBUG_MODE 1
#endif

typedef float DType;
using Eigen::Tensor;
using Eigen::Dynamic;
using Eigen::Vector3f;
using std::stringstream;

struct Buffers {
    enum Buffer{
        Color = 1,
        Depth = 2,
    };

    int buffer;
    explicit Buffers(Buffer buf):buffer(buf) {}
    Buffers(int buf):buffer(buf) {}
    operator int() const {return static_cast<int>(buffer);}
};

class Rasterizer {
public:
    enum class DrawingMode {Wireframe, Triangle};

    typedef int PositionIndex;
    typedef int IndicesIndex;
    typedef int ColorIndex;

    typedef float ZBufferType;

private:
    int width_, height_;
    Tensor<DType, 3> frame_buffer_; // 3-dim tensor
    Eigen::Matrix<ZBufferType, Dynamic, Dynamic> z_buffer_;
    Eigen::Matrix4f model_, view_, projection_;


public:
    Rasterizer(int h,int w);
    Rasterizer(): Rasterizer(0,0) {};

    //getters
    const int width() const {return width_;}
    const int height() const {return height_;}
    const auto & framebuffer() {return frame_buffer_;}
    const float getZBuffer(int x,int y) {return z_buffer_(y,x);}
    std::tuple<int, int> get_framebuffer_shape() {
        return {frame_buffer_.dimension(0),frame_buffer_.dimension(1)};
    }

    //setters
    void clearBuffer(Buffers );
    void setModel(const Eigen::Matrix4f &model)         {model_ = model;}
    void setView(const Eigen::Matrix4f &view)           {view_ = view;}
    void setProjection(const Eigen::Matrix4f &project)  {projection_ = project;}
    inline void setPixel(int x,int y,const ColorType& color);
    inline void setDepth(int x,int y,ZBufferType z);

    void draw(const std::vector<Triangle> &triangles);
    void drawLine(Vector3f begin,Vector3f end);
    void drawTriangle(const Triangle &tri);

};


inline void Rasterizer::setPixel(int x,int y,const ColorType& color) {
#if DEBUG_MODE
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
#if DEBUG_MODE
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
