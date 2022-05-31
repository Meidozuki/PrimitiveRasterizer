//
// Created on 2022/5/28.
//

//#pragma GCC optimize("O1")

#include <iostream>
#include <ctime>
#include <random>

#include "gtest/gtest.h"
#include "rasterizer.cpp"

#define DEBUG_MODE 1

using namespace std;

class TimeCompareFixture : public ::testing::Test{
 protected:
    long long int n_repeat;
    default_random_engine e;
    uniform_int_distribution<int> u;

    Triangle triangle;
    Rasterizer r;

    virtual void SetUp () {
        n_repeat=3000;
        e=default_random_engine(clock());
        u=uniform_int_distribution<int> (100,700);
        r=Rasterizer(1200,900);


        triangle.setVertex(0,{u(e),u(e),0});
        triangle.setVertex(1,{u(e),u(e),0});
        triangle.setVertex(2,{u(e),u(e),0});

    }
    virtual void TearDown () {}
};


void drawTriangle(Rasterizer &r,const Triangle &tri,bool mode) {
    ColorType color{1,1,1};
    Eigen::Matrix3f vertex_data;
    vertex_data << tri.vertex_[0], tri.vertex_[1], tri.vertex_[2];

    Vector3f &&min_v = vertex_data.rowwise().minCoeff();
    int infX = std::floor(min_v.x()), infY = std::floor(min_v.y());
    Vector3f &&max_v = vertex_data.rowwise().maxCoeff();
    int supX = std::floor(max_v.x()), supY = std::floor(max_v.y());
//    std::cout << infX << ' ' << infY << ' ' << supX << ' ' << supY << std::endl;

    for (int i = infX;i < supX;++i) {
        for (int j = infY;j < supY;++j) {
            if (mode == false) {
                if (!insideTriangle(i,j,tri.vertex_)) {
                    continue;
                }
                auto [alpha, beta, det] = computeBarycentric2D(Vector3f(i,j,0.0), tri.vertex_);
            }
            else {
                auto [alpha, beta, det] = computeBarycentric2D(Vector3f(i,j,0.0), tri.vertex_);
                if (std::abs(det) < 1e-5) {
                    std::cerr << "drawing a line-shape triangle.\n";
                    return;
                }
                alpha /= det, beta /= det;
                float gamma = 1-alpha-beta;
                if (!insideTriangle_Barycentric(alpha,beta,gamma)) {
                    continue;
                }

            }

            r.setPixel(i,j,color);
        }
    }
}

TEST_F(TimeCompareFixture, cross) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        triangle.setVertex(0,{u(e),u(e),0});
        triangle.setVertex(1,{u(e),u(e),0});
        triangle.setVertex(2,{u(e),u(e),0});
        drawTriangle(r,triangle,false);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}


TEST_F(TimeCompareFixture, interpo) {
    int i;
    clock_t begin,end;
    begin=clock();
    for (i=0;i < n_repeat;++i) {
        triangle.setVertex(0,{u(e),u(e),0});
        triangle.setVertex(1,{u(e),u(e),0});
        triangle.setVertex(2,{u(e),u(e),0});
        drawTriangle(r,triangle,true);
    }
    end=clock();
    cout << "time: " << end-begin << endl;
}

int main (int argc,char** argv) {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}