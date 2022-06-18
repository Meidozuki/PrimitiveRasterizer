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
#include "voxel/polygon.hpp"

#if 1
  #include "debug_test_example.cpp"
#endif

using std::cout;
using std::endl;

int main() {
    std::default_random_engine e;
    std::uniform_real_distribution<float> u(0,1);


    const int height=600,width=800;
    Rasterizer raster(height,width);

    float angle = 10;
    raster.setModel(getModelMatrix(angle));
    raster.setView(getViewMatrix({0,1.5,3}));
    raster.setProjection(GetProjectionMatrix(60, 1, 0.1, 10));

    std::vector<Triangle> triangle_list;

    ObjLoader obj_loader;
    obj_loader.loadObjFile("../cow.obj");

    auto& vertices=obj_loader.vertices_buf;
    for (Eigen::Vector3i &index: obj_loader.indices_buf) {
        Triangle tri;
        for (int i=0;i < 3;++i) {
            int idx=index[i];
            auto vert=vertices[idx];
            tri.setVertex(i, vert);
        }
        float col=tanh(tri.vertex_[0].z())/2+0.5;
        col=1;
        tri.setColor(0,{0,0,col});
        tri.setColor(1,{0,0,col});
        tri.setColor(2,{0,0,col});
        triangle_list.push_back(std::move(tri));
    }
    raster.draw(triangle_list);

    int key =0;
    while (key != 27) {
        raster.clearBuffer(Buffers::Color | Buffers::Depth);

        raster.setModel(getModelMatrix(angle));
        raster.setEyePos({0,1,3});
        raster.setProjection(GetProjectionMatrix(60, 1, 0.1, 10));


//        test_triangle();
//        test_cone();


        float startX=-0.2,startY=0.2,startZ=-0.2, cube_size=0.12;
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

        voxel::Mesh::setMeshColor({0,0.5,0.7});

        raster.draw(triangle_list);
        triangle_list.clear();


        cv::Mat image(height,width,CV_32FC3);
        cv::eigen2cv(raster.framebuffer(),image);
        cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
        cv::flip(image,image,0); //竖直翻转
        cv::imshow("image",image);

        key = cv::waitKey(0);
        angle += 10;
    }

    return 0;
}
