//
// Created by user on 2022/6/2.
//

#include "obj_loader.hpp"

#include <stdexcept>
#include <algorithm>

void ObjLoader::loadObjFile(std::string filename) {
    std::ifstream fs(filename);
    std::string oneline, keyword;
    int f_mode=-1;

    while (fs.is_open() && !fs.eof()) {
        std::getline(fs,oneline);
        std::stringstream ss;
        ss << oneline;
        ss >> keyword;
        if (oneline.length() == 0 || keyword == "#") {
            continue;
        }

        else if (keyword == "v") {
            //vertex
            float x,y,z;
            ss >> x >> y >> z;
            addVertex(x,y,z);
        }
        else if (keyword == "vt") {
            float u,v,w;
            ss >> u >> v; ss.ignore();
            if (!ss.eof()) ss >> w;
        }
        else if (keyword == "vn") {
            throw std::runtime_error("Not implemented");
        }
        else if (keyword == "f") {
            if (f_mode == -1) {
                int cnt = std::count(oneline.begin(),oneline.end(),'/');
                if (cnt % 3 != 0) {
                    throw std::runtime_error("The number of / in f line is 3*k.\n");
                }
                f_mode = cnt / 3;
            }

            int vs[3],vts[3],vns[3];
            char c;
            for (int i=0;i < 3;++i) {
                switch (f_mode) {
                    case 0:
                        ss >> vs[i];
                        break;
                    case 1:
                        ss >> vs[i] >> c >> vts[i];
                        break;
                    case 2:
                        ss >> vs[i] >> c >> vts[i] >> c >> vns[i];
                        break;
                    default:
                        throw std::out_of_range(".obj f format error.\n");
                }
            }

            //TODO:处理纹理
            addIndex(vs[0],vs[1],vs[2]);
        }
        else {
            std::cerr << "Not recognized: " << oneline << std::endl;
        }
    }
}