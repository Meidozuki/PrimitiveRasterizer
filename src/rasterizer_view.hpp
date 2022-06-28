//
// Created by user on 2022/6/28.
//

#ifndef RASTERIZER_RASTERIZER_VIEW_HPP
#define RASTERIZER_RASTERIZER_VIEW_HPP

#include <string>

#include <Eigen/Core>
#include <opencv2/core/eigen.hpp>

#include "rasterizer.hpp"

void show_img(Rasterizer &raster, const std::string &title = "image");

cv::Mat eigen2cv_(Rasterizer &raster);

#endif //RASTERIZER_RASTERIZER_VIEW_HPP
