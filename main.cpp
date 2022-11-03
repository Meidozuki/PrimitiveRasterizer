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

#if 0
  #include "debug_test_example.cpp"
#endif

using std::cout;
using std::endl;

int main() {
//    std::default_random_engine e;
//    std::uniform_real_distribution<float> u(0,1);


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

    {
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
    }





    ViewModel vm;
    vm.setModel(std::make_unique<VMModel>());

    int key =0;
    while (key != 27 && key != '\b') {
        vm.clear_cmd_.execute();



//        test_triangle();
//        test_cone();


        vm.draw(triangle_list);
        vm.show();

        key = cv::waitKey(100);
//        cout << "key " << key << endl;
        vm.rotate_cmd_.setParameter(std::string_view("plus"));
        vm.rotate_cmd_.execute();

    }

//    vm.model_.saveImage();

    return 0;
}
