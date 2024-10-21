#ifndef __OPENRM_RESIZE_CUH__
#define __OPENRM_RESIZE_CUH__
#include "cudatools.h"

//定义了一个名为AffineMatrix的结构体，用于存储仿射变换矩阵的6个参数
struct AffineMatrix {
    //float value[6];
    cv::Mat matrix;
};

extern AffineMatrix input_to_infer;
extern AffineMatrix infer_to_input;

//计算两个中心点角度
float calculate_rotate_angle(const cv::Point2f& src_center,const cv::Point2f& dst_center);

//用于生成仿射变换矩阵
void generate_affine_matrix(int src_width, int src_height, int dst_width, int dst_height);

//warpaffine_kernel是一个CUDA核函数，用于执行图像的仿射变换

//void warpaffine_kernel(
//    uint8_t* src,
//    int src_line_size,
//    int src_width,
//    int src_height,
//    float* dst,
//    int dst_width,
//    int dst_height,
//    uint8_t const_value_st,
//    AffineMatrix d2s,
//    int edge
//);

// 使用 OpenCV 的 warpAffine 函数来执行图像的仿射变换
void warpaffine_kernel(const cv::Mat& src, cv::Mat& dst, const AffineMatrix& matrix);

#endif