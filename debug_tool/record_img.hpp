//
// Created by user on 2022/7/26.
//

#ifndef RASTERIZER_RECORD_IMG_HPP
#define RASTERIZER_RECORD_IMG_HPP

#include <filesystem>
#include <string>
#include <optional>

#include "rasterizer.hpp"

namespace debug_img {
    void save_img(Rasterizer &r,std::optional<std::string> filename = std::nullopt);
    void clear_dir(std::string path);
}

#endif //RASTERIZER_RECORD_IMG_HPP
