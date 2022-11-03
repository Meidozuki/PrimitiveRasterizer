//
// Created by user on 2022/7/26.
//

#ifndef RASTERIZER_IMAGE_IO_HPP
#define RASTERIZER_IMAGE_IO_HPP

#include <filesystem>
#include <string>
#include <optional>

#include <Eigen/Core>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core.hpp>

#include "rasterizer.hpp"

namespace imageio {
    using std::filesystem::exists;
    using std::filesystem::create_directory;

    void save_img(Rasterizer &r,std::optional<std::string> filename = std::nullopt);
    void clear_dir(std::string path);


    void show_img(const Rasterizer &raster, const std::string &title = "image");
    cv::Mat eigen2cv_(const Rasterizer &raster);
}

#endif //RASTERIZER_IMAGE_IO_HPP
