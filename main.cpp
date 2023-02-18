#include <iostream>
#include <string>
#include <vector>
#include <random>

#include <Eigen/Core>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "rasterizer.hpp"
#include "image_io.hpp"
#include "obj_loader.hpp"
#include "voxel/polygon.hpp"
#include "MVVM/model/model_interface.hpp"
#include "MVVM/viewmodel/viewmodel.hpp"

//#include "debug_test_example.cpp"

using std::cout;
using std::endl;

int main() {
    std::vector<Triangle> triangle_list;

    ObjLoader obj_loader;
    obj_loader.loadObjFile("../cow.obj");

//    auto& vertices=obj_loader.vertices_buf;
//    for (Eigen::Vector3i &index: obj_loader.indices_buf) {
//        Triangle tri;
//        for (int i=0;i < 3;++i) {
//            int idx=index[i]-1;
//            auto vert=vertices[idx];
//            tri.setVertex(i, vert);
//        }
//        float col=tanh(tri.vertex(0).z())/2+0.5;
//        col=1;
//        tri.setColor(0,{0,1,col});
//        tri.setColor(1,{0,1,col});
//        tri.setColor(2,{0,1,col});
//        tri.setNormals({});
//        triangle_list.push_back(std::move(tri));
//    }
//    raster.draw(triangle_list);

//    {
//    float startX=-0.2,startY=1,startZ=-0.2, cube_size=0.1;
//    int n_cubes=5;
//
//    for (int i=0;i < n_cubes;++i) {
//        for (int j=0;j < n_cubes;++j) {
//            float left = startX + cube_size*2*i;
//            float bottom = startY + cube_size*2*j;
//
//            ColorType color(1.0-(1.0*i/n_cubes),i*cube_size,(1.0*i/n_cubes));
//            voxel::Mesh2D::setMeshColor(color);
//            voxel::Cube cube(left,bottom,startZ,left+cube_size,bottom+cube_size,startZ+cube_size);
//            cube.getTriangles(triangle_list);
//        }
//    }

//    voxel::Mesh2D::setMeshColor({1, 0, 0});
//
//    voxel::Sphere ball(Vector3f(0,0,0.5),0.4,100,20);
//    ball.getTriangles(triangle_list);
//    }

    voxel::TriangleMesh::setMeshColor({1,0,0});

    VMModel_Debug model;
    voxel::Rectangle rect({0.2, 0.2},0.2,0.2);
    voxel::Circle circle({0,0},0.3);
    model.clearBuffer(Buffers::Color | Buffers::Depth);
    model.draw2D(rect);
    model.draw2D(circle);
    imageio::show_img(model.asRasterizer());
    cv::waitKey();
    return 0;



    ViewModel vm;
    vm.setModel(std::make_unique<VMModel>());

    int key =0;
    while (key != 27 && key != '\b') {
        vm.clear_cmd_.execute();



//        test_triangle();
//        test_cone();


        vm.draw(triangle_list);
        vm.show();

        key = cv::waitKey();
//        cout << "key " << key << endl;
        vm.rotate_cmd_.setParameter(std::string_view("plus"));
        vm.rotate_cmd_.execute();

    }

//    vm.model_.saveImage();

    return 0;
}
