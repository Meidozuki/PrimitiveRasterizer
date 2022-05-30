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

#include "triangle.hpp"

#ifndef DEBUG_MODE
  #define DEBUG_MODE 1
#endif

typedef float DType;
typedef Eigen::Vector3f ColorType;
using Eigen::Tensor;
using Eigen::Dynamic;
using Eigen::Vector3f;
using std::stringstream;

class Rasterizer {
public:
    enum class drawing_mode {Wireframe, Triangle};

    typedef int PositionIndex;
    typedef int IndicesIndex;
    typedef int ColorIndex;

    typedef float ZBufferType;

private:
    int width_, height_;
    Tensor<DType, 3> frame_buffer_; // 3-dim tensor
    Eigen::Matrix<ZBufferType, Dynamic, Dynamic> z_buffer_;

    std::map<PositionIndex, std::vector<Vector3f> > pos_buf;
    std::map<IndicesIndex , std::vector<Vector3f> > idx_buf;
    std::map<ColorIndex   , std::vector<Vector3f> > color_buf;

public:
    Rasterizer(int h,int w): width_(w),height_(h) {
        frame_buffer_.resize({height_, width_, 3});
    }
    Rasterizer(): Rasterizer(0,0) {};

    //getters
    const int width() const {return width_;}
    const int height() const {return height_;}
    const auto & framebuffer() {return frame_buffer_;}
    std::tuple<int, int> get_framebuffer_shape() {
        return {frame_buffer_.dimension(0),frame_buffer_.dimension(1)};
    }

    inline void setPixel(int x,int y,const ColorType& color);

    void drawLine(Vector3f begin,Vector3f end);
    void drawTriangle(const Triangle &);

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

    frame_buffer_(x, y, 0) = color[0];
    frame_buffer_(x, y, 1) = color[1];
    frame_buffer_(x, y, 2) = color[2];
}


#endif //MAIN_CPP_RASTERIZER_HPP
