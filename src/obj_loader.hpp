//
// Created by user on 2022/6/2.
//

#ifndef RASTERIZER_OBJ_LOADER_HPP
#define RASTERIZER_OBJ_LOADER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <Eigen/Core>

class ObjLoader {
public:
    std::vector<Eigen::Vector3f> vertices_buf;
    std::vector<Eigen::Vector3f> vertex_texture_buf;
    std::vector<Eigen::Vector3f> vertex_normal_buf;
    std::vector<Eigen::Vector3i> indices_buf;

    void addVertex (float a, float b, float c) {
        Eigen::Vector3f v(a,b,c);
        vertices_buf.push_back(std::move(v));
    }
    void addVertexTexture(float u, float v, float w=0.0f) {
        Eigen::Vector3f tex(u,v,w);
        vertex_texture_buf.push_back(std::move(tex));
    }
    void addIndex (int a, int b, int c) {
        Eigen::Vector3i v(a,b,c);
        indices_buf.push_back(std::move(v));
    }

    void loadObjFile(std::string filename);
};

#endif //RASTERIZER_OBJ_LOADER_HPP
