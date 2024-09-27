#ifndef __OPENRM_CUDA_TOOLS_H__
#define __OPENRM_CUDA_TOOLS_H__
#include <cstdint>
#include <opencv2/opencv.hpp>
namespace rm {
//用于图像缩放的函数
/*
void resize(
    uint8_t* src,
    int src_width,
    int src_height,
    float* dst,
    int dst_width,
    int dst_height,
    void* cuda_stream
);
*/
    void resize(const cv::Mat& src, cv::Mat& dst, int dst_width, int dst_height);
}

#endif