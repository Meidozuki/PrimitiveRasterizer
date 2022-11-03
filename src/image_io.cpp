//
// Created by user on 2022/7/26.
//

#include "image_io.hpp"

#include <iostream>
#include <vector>
#include <sstream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "config.h"
#include "rasterizer.hpp"

namespace filesystem = std::filesystem;

namespace imageio {
    short int auto_cnt = 0;

    cv::Mat eigen2cv_(const Rasterizer &raster) {
        cv::Mat image(raster.height(), raster.width(), CV_32FC3);
        cv::eigen2cv(raster.framebuffer(),image);
        cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
        cv::flip(image,image,0); //竖直翻转
        return image;
    }

    void show_img(const Rasterizer &raster, const std::string &title) {
        cv::Mat image = eigen2cv_(raster);
//        image.convertTo(image,CV_8UC3);
        cv::imshow(title,image);
    }

    void save_img(Rasterizer &r,std::optional<std::string> filename) {
        if (!filename.has_value()) {
            stringstream ss;
            ss << auto_cnt++;
            filename = ss.str();
            filename = std::string(PROJ_SRC_DIR) + "/debug_tool/cache/" + filename.value() + ".tiff";
        }

        auto &fname = filename.value();
        cv::Mat image = eigen2cv_(r);

        if (!exists(fname + "/..")) {
            create_directory(fname + "/..");
        }

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