//
// Created on 2022/6/4.
//

#include <vector>
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

void test_cone(){
    const int height=300,width=400;
    Rasterizer raster(height,width);
    raster.clearBuffer(Buffers::Color | Buffers::Depth);

    raster.setModel(getModelMatrix(0));
    raster.setView(getViewMatrix({0,1.4,3}));
    raster.setProjection(GetProjectionMatrix(60, 1, 0.1, 10));

    std::vector<Triangle> triangle_list;
    voxel::Mesh::setMeshColor({0,0.5,0.7});

    voxel::Cone cone(16,{0,0.5,0},0.5);
    cone.getTriangles(triangle_list);
    voxel::Cone cone2(16,{0,0.5,0},0.5,-1);
    cone2.getTriangles(triangle_list);

    raster.draw(triangle_list);

    cv::Mat image(height,width,CV_32FC3);
    cv::eigen2cv(raster.framebuffer(),image);
    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    cv::flip(image,image,0); //竖直翻转
    cv::imshow("cone test",image);
}

void test_cubes(){
    const int height=300,width=400;
    Rasterizer raster(height,width);
    raster.clearBuffer(Buffers::Color | Buffers::Depth);

    raster.setModel(getModelMatrix(30));
    raster.setView(getViewMatrix({0,1,2}));
    raster.setProjection(GetProjectionMatrix(60, 1, 0.1, 10));

    std::vector<Triangle> triangle_list;

    float startX=-0.2,startY=0.2,startZ=-0.2, cube_size=0.1;
    int n_cubes=5;

    for (int i=0;i < n_cubes;++i) {
        for (int j=0;j < n_cubes;++j) {
            float left = startX + cube_size*2*i;
            float bottom = startY + cube_size*2*j;

            ColorType color(1.0-(1.0*i/n_cubes),i*cube_size,(1.0*i/n_cubes));
            voxel::Mesh::setMeshColor(color);
            voxel::Cube cube(left,bottom,startZ,left+cube_size,bottom+cube_size,startZ+cube_size);
            cube.getTriangles(triangle_list);
        }
    }

    raster.draw(triangle_list);

    cv::Mat image(height,width,CV_32FC3);
    cv::eigen2cv(raster.framebuffer(),image);
    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    cv::flip(image,image,0); //竖直翻转
    cv::imshow("cube test",image);
}