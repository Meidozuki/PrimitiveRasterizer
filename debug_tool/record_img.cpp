//
// Created by user on 2022/7/26.
//

#include "record_img.hpp"

#include <iostream>
#include <vector>
#include <sstream>

#include <Eigen/Core>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "config.h"
#include "rasterizer.hpp"
#include "rasterizer_view.hpp"

namespace filesystem = std::filesystem;

namespace debug_img {
    short int auto_cnt = 0;

    void save_img(Rasterizer &r,std::optional<std::string> filename) {
        if (!filename.has_value()) {
            stringstream ss;
            ss << auto_cnt++;
            filename = ss.str();
        }

        auto fname = std::string(PROJ_SRC_DIR) + "/debug_tool/cache/" + filename.value() + ".tiff";
        cv::Mat image = eigen2cv_(r);
        cv::imwrite(fname,image);
    }

    void clear_dir(std::string path) {
        std::vector<std::string> result;
        cv::glob(path,result);

        for (auto &name: result) {
            bool suc = filesystem::remove(name);
            if (!suc) {
                std::cerr << "File " << name << "removing failed." << std::endl;
            }
        }
    }

}