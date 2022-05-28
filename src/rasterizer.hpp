//
// Created on 2022/5/27.
//

#ifndef MAIN_CPP_RASTERIZER_HPP
#define MAIN_CPP_RASTERIZER_HPP

#include <iostream>
#include <stdexcept>
#include <tuple>

#include <Eigen/Core>
#include "unsupported/Eigen/CXX11/Tensor"

#define DEBUG_MODE 1
#if DEBUG_MODE
#include <sstream>
using std::stringstream;
#endif

typedef float DType;
typedef Eigen::Vector3f ColorType;
using Eigen::Tensor;
using Eigen::Dynamic;

class Rasterizer {
private:
    int width_, height_;
    Tensor<DType, 3> frame_buffer_; // 3-dim tensor

public:
    Rasterizer(int h,int w): width_(w),height_(h) {
        frame_buffer_.resize({height_, width_, 3});
    }
    Rasterizer(): Rasterizer(0,0) {};

    //getters
    const int width() const {return width_;}
    const int height() const {return height_;}
    const auto & framebuffer() {return frame_buffer_;}
//    std::tuple<int, int> get_framebuffer_shape() {return {frame_buffer_.size(),frame_buffer_.cols()};}

    inline void setPixel(int x,int y,const ColorType& color);

    void drawLine(Eigen::Vector3f begin,Eigen::Vector3f end);
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

    frame_buffer_(x,y,0)=color[0];
    frame_buffer_(x,y,1)=color[1];
    frame_buffer_(x,y,2)=color[2];
}


#endif //MAIN_CPP_RASTERIZER_HPP
