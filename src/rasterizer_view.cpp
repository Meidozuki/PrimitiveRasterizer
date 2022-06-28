//
// Created by user on 2022/6/28.
//

#include "rasterizer_view.hpp"


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

cv::Mat eigen2cv_(Rasterizer &raster) {
    cv::Mat image(raster.height(), raster.width(), CV_32FC3);
    cv::eigen2cv(raster.framebuffer(),image);
    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    cv::flip(image,image,0); //竖直翻转
    return image;
}


void show_img(Rasterizer &raster, const std::string &title) {
    cv::Mat image = eigen2cv_(raster);
    cv::imshow(title,image);
}