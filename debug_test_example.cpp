//
// Created on 2022/6/4.
//

#include <Eigen/Core>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "basic_matrix.hpp"
#include "rasterizer.hpp"

void test_triangle(){
    const int height=300,width=400;
    Rasterizer raster(height,width);
    raster.clearBuffer(Buffers::Color | Buffers::Depth);

    raster.setModel(getModelMatrix(0));
    raster.setView(getViewMatrix({0,0,3}));
    raster.setProjection(GetProjectionMatrix(60, 1, 0.1, 10));

    Triangle triangle;
    triangle.setVertex(0,{1,1,0});
    triangle.setVertex(1,{1,0,0});
    triangle.setVertex(2,{0,0,0});
    triangle.setColor(0,{1,0,0});
    triangle.setColor(1,{0,0,1});
    triangle.setColor(2,{1,1,1});
    raster.draw({triangle});

    cv::Mat image(height,width,CV_32FC3);
    cv::eigen2cv(raster.framebuffer(),image);
    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    cv::flip(image,image,0); //竖直翻转
    cv::imshow("triangle test",image);
}