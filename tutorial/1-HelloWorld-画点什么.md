# 第一份代码

~~~cpp
#include <iostream>
#include <Eigen/Core>
#include <opencv2/core.hpp>

int main() {
    std::cout << "Hello World!" << std::endl;
}
~~~

在试着运行上述代码后，如果没有报错，说明已经基本成功引入了两个库（说基本是因为这一步没有实际跑opencv，还没检查dll）

接下来在项目目录下创建一个src文件夹，用来存放代码。为了在写代码时缩短#include "src/xx.cpp"为#include "xx.cpp"，我们要修改一下cmakelists让cmake能够将src文件夹添加到路径中

```cmake
cmake_minimum_required(VERSION 3.16)
project(Rasterizer)
set(CMAKE_CXX_STANDARD 17) #设置为C++17标准

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    #eigen package
    include_directories("3Party/eigen/include/eigen3")
    #opencv package
    find_package(OpenCV REQUIRED
            PATHS 3Party/opencv)
endif()
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(src) #将src添加到路径
file(GLOB SRC_CPP
        src/*.cpp)
list(APPEND SRC_CPP
        main.cpp)
add_executable(Rasterizer ${SRC_CPP})

target_link_libraries(Rasterizer ${OpenCV_LIBS})
```

之后的项目将基于该cmakelists进行变动（使用glob文件可能导致在代码自动生成头保护宏时生成的格式不同，并且每次添加新文件后仍需要重新加载cmakelists，比较推荐在写完代码后缩减cmakelists长度时再加）。

接下来让我们试着创建一个窗口

```cpp
// main.cpp
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main() {
    std::cout << "Hello World!" << std::endl;

    const int height=300,width=400;
    cv::Mat image(height,width,CV_32FC3);
    cv::imshow("image",image);
}

```

这时候我们能看到一个黑色的窗口，那让我们试着画一条线吧

```cpp
// main.cpp
#include <iostream>

#include <Eigen/Core>

#include <opencv2/core/eigen.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "rasterizer.hpp"

cv::Mat eigen2cv_(Rasterizer &raster) {
        return image;
    }

int main() {
    std::cout << "Hello World!" << std::endl;

    const int height=300,width=400;
    Rasterizer raster(height,width);

    raster.drawLine({100,100,0},{200,200,0});

    cv::Mat image(raster.height(), raster.width(), CV_32FC3);
    cv::eigen2cv(raster.framebuffer(),image);
    cv::cvtColor(image,image,cv::COLOR_BGR2RGB);
    cv::flip(image,image,0); //竖直翻转
    cv::imshow("image",image);
}


```

我们定义了一个光栅化渲染器raster，然后让它画一条线。接下来用opencv的接口将Eigen tensor转为cv::Mat，然后使用cv::imshow显示图片。接下来我们转到具体实现。

这里我们先使用DDA算法来划线（GAMES101友情链接：https://sites.cs.ucsb.edu/~lingqi/teaching/games101.html）。简单来说，DDA算法就是用让x以1为步长，每一次计算y的累加值，四舍五入后绘制（x,y）点。更详细的内容可以看其他CG资料。

然后在src文件夹下创建rasterizer.cpp和straight_line.cpp

```cpp
//rasterizer.cpp

#include <array>
#include <Eigen/Core>
#include "unsupported/Eigen/CXX11/Tensor"

#include "straight_line.hpp"

using Eigen::Tensor;
using Eigen::Dynamic;
using Eigen::Vector3f;

class Rasterizer {
protected:
    Tensor<DType, 3> frame_buffer_; // 3-dim eigen tensor
    Eigen::Matrix<ZBufferType, Dynamic, Dynamic> z_buffer_;

    int width_, height_;
    Eigen::Matrix4f model_, view_, projection_;

    Shader shader_;

public:
    using ZBufferType = float;
    Rasterizer(int h,int w);
    Rasterizer(): Rasterizer(0,0) {};

    //getters
    int width() const {return width_;}
    int height() const {return height_;}
    const auto & framebuffer() {return frame_buffer_;}
    float getZBuffer(int x,int y) {return z_buffer_(y,x);}
    std::tuple<int, int> get_framebuffer_shape() {
        return {frame_buffer_.dimension(0),frame_buffer_.dimension(1)};
    }
};

//————————Constructor
Rasterizer::Rasterizer(int h, int w): width_(w),height_(h) {
    frame_buffer_.resize({height_, width_, 3});
    z_buffer_.resize(height_, width_);
    clearBuffer(Buffers::Depth | Buffers::Color);

    model_ = view_ = projection_ = Eigen::Matrix4f::Identity();
}

//————————drawing
void Rasterizer::drawLine(const Vector3f & begin, const Vector3f & end, const ColorType &color) {
    line_drawing::drawLine(begin,end,color,*this);
}
```

```cpp
// straight_line.cpp

namespace line_drawing {
void drawLine_DigitalDifferentialAnalyzer(int x1, int y1, int x2, int y2, Rasterizer &r) {
    int dx = x2 - x1, dy = y2 - y1;
    ColorType color{1.0f, 1.0f, 1.0f};

    int n_step = std::abs(dx) > std::abs(dy) ? std::abs(dx) : std::abs(dy);
    float delta_x = 1.0f * dx / n_step, delta_y = 1.0f * dy / n_step;
    float x = x1, y = y1;

    for (int i = 0; i < n_step; ++i) {
        x = x + delta_x;
        y = y + delta_y;
        r.setPixel(x, y, color);
    }
}


void drawLine(Eigen::Vector3f begin,Eigen::Vector3f end, const ColorType &color, Rasterizer &r) {
    drawLine_DigitalDifferentialAnalyzer(begin.x(),begin.y(),end.x(),end.y(),r);
}

}
```

## Bresenham法

上述的绘制直线方法仍然有些问题，可以选用Bresenham法绘制直线（之后补）。

v0.2版本的直线绘制算法在[这里](./code_snapshot/1-appdix-straight_line.cpp)
