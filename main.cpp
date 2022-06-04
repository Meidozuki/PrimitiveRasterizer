#include <iostream>
#include <string>
#include <vector>
#include <random>

#include <Eigen/Core>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "basic_matrix.hpp"
#include "rasterizer.hpp"
#include "obj_loader.hpp"
#include "src/voxel/polygon.hpp"

#if 1
  #include "debug_test_example.cpp"
#endif

using std::cout;
using std::endl;

int main() {
    std::default_random_engine e;
    std::uniform_real_distribution<float> u(0,1);

    std::cout << "Hello, World!" << std::endl;

    const int height=300,width=400;
    Rasterizer raster(height,width);
    raster.clearBuffer(Buffers::Color | Buffers::Depth);

    std::vector<Triangle> triangle_list;

    ObjLoader obj_loader;
    obj_loader.loadObjFile("../cow.obj");

    raster.setModel(getModelMatrix(30));
    raster.setView(getViewMatrix({0,0,3}));
    raster.setProjection(GetProjectionMatrix(60, 1, 0.1, 10));

//    auto& vertices=obj_loader.vertices_buf;
//    for (Eigen::Vector3i &index: obj_loader.indices_buf) {
//        Triangle tri;
//        for (int i=0;i < 3;++i) {
//            int idx=index[i];
//            auto vert=vertices[idx];
//            tri.setVertex(i, vert);
//        }
//        float col=tanh(tri.vertex_[0].z())/2+0.5;
//        col=1;
//        tri.setColor(0,{0,0,col});
//        tri.setColor(1,{0,0,col});
//        tri.setColor(2,{0,0,col});
//        triangle_list.push_back(std::move(tri));
//    }
//    raster.draw(triangle_list);

    test_triangle();

    triangle_list.clear();
//    voxel::Rectangle rect(0.9,0,0,0.9);
    voxel::Cube rect(0,0,0,0.5,0.5,0.5);
    rect.getTriangles(triangle_list);
    raster.draw(triangle_list);


    cv::Mat image(height,width,CV_32FC3);
    cv::eigen2cv(raster.framebuffer(),image);
    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    cv::flip(image,image,0); //竖直翻转
    cv::imshow("image",image);

    cv::waitKey();

    return 0;
}
