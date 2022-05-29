#include <iostream>

#include <Eigen/Core>

#include <opencv2/core/eigen.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "basic_matrix.hpp"
#include "rasterizer.hpp"

using std::cout;
using std::endl;

int main() {
    std::cout << "Hello, World!" << std::endl;

    const int height=300,width=400;
    Rasterizer raster(height,width);

    Triangle triangle;
    triangle.setVertex(0,{50,50,0});
    triangle.setVertex(1,{50,100,0});
    triangle.setVertex(2,{100,50,0});
    raster.drawTriangle(triangle);

    raster.drawLine({100,100,0},{200,250,0});

    cv::Mat image(height,width,CV_32FC3);
    cv::eigen2cv(raster.framebuffer(),image);
    cv::cvtColor(image,image,cv::COLOR_RGB2GRAY);
    cv::imshow("image",image);

    cv::waitKey();

    return 0;
}
