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
    raster.clearBuffer(Buffers::Color | Buffers::Depth);


    raster.setModel(get_model_matrix(10));
    raster.setView(get_view_matrix({0,0,10}));
    raster.setProjection(get_projection_matrix(45.0, 1, 0.1, 50));

    Triangle triangle;
    triangle.setVertex(0,{2,0,-2});
    triangle.setVertex(1,{2,-2,0});
    triangle.setVertex(2,{-2,0,-2});
    triangle.setColor(0,{1,0,0});
    triangle.setColor(1,{0,1,0});
    triangle.setColor(2,{0,0,1});
    raster.draw({triangle});


    cv::Mat image(height,width,CV_32FC3);
    cv::eigen2cv(raster.framebuffer(),image);
    cv::imshow("image",image);

    cv::waitKey();

    return 0;
}
