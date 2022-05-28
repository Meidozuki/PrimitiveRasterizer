//
// Created on 2022/5/27.
//

#ifndef MAIN_CPP_RASTERIZER_HPP
#define MAIN_CPP_RASTERIZER_HPP

#include <iostream>
#include <stdexcept>
#include <tuple>

#include <Eigen/Core>

#define DEBUG_MODE 1
#if DEBUG_MODE
#include <sstream>
using std::stringstream;
#endif

typedef Eigen::Vector3f ColorType;
using Eigen::Dynamic;

class Rasterizer {
private:
    int width_, height_;
    Eigen::Matrix<ColorType, Dynamic, Dynamic> frame_buffer_;

public:
    Rasterizer(int h,int w): width_(w),height_(h) {
        frame_buffer_.resize(height_,width_);
    }
    Rasterizer(): Rasterizer(0,0) {};

    const int width() const {return width_;}
    const int height() const {return height_;}
    std::tuple<int, int> get_framebuffer_shape() {return {frame_buffer_.rows(),frame_buffer_.cols()};}

    inline void setPixel(int x,int y,const ColorType& color);


};

void Rasterizer::setPixel(int x,int y,const ColorType& color) {
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

    frame_buffer_(x,y)=color;
}


#endif //MAIN_CPP_RASTERIZER_HPP
