#include "resize.h"

#include <algorithm>

#include <cmath>

AffineMatrix input_to_infer;

AffineMatrix infer_to_input;

//void generate_affine_matrix(int src_width, int src_height, int dst_width, int dst_height) {

//    // 获取比例系数

//    float scaleIN = std::min(static_cast<float>(dst_height) / src_height, static_cast<float>(dst_width) / src_width);

//		float scaleOUT = 1.f / scaleIN;

//

//    // 计算从input到infer仿射变换矩阵各点的值

//    input_to_infer.value[0] = scaleIN;

//    input_to_infer.value[1] = 0;

//    input_to_infer.value[2] = -scaleIN * src_width * 0.5 + dst_width * 0.5;

//    input_to_infer.value[3] = 0;

//    input_to_infer.value[4] = scaleIN;

//    input_to_infer.value[5] = -scaleIN * src_height * 0.5 + dst_height * 0.5;

//

//    // 计算从infer到input仿射变换矩阵各点的值

//    infer_to_input.value[0] = scaleOUT;

//    infer_to_input.value[1] = 0;

//    infer_to_input.value[2] = -scaleOUT * dst_width * 0.5 + src_width * 0.5;

//    infer_to_input.value[3] = 0;

//    infer_to_input.value[4] = scaleOUT;

//    infer_to_input.value[5] = -scaleOUT * dst_height * 0.5 + src_height * 0.5;

//}

//根据两个中心点求旋转的角度
float calculate_rotate_angle(const cv::Point2f& src_center,const cv::Point2f& dst_center)
{
    //两个中心点之间的向量
    cv::Point2f vector = dst_center - src_center;
    float angle = atan2(vector.y,vector.x) * 180 / CV_PI;
    return angle;

}

// 用于生成仿射变换矩阵
void generate_affine_matrix(int src_width, int src_height, int dst_width, int dst_height) {

    // 获取中心点
    cv::Point2f src_center(src_width * 0.5f, src_height * 0.5f);
    cv::Point2f dst_center(dst_width * 0.5f, dst_height * 0.5f);

    // 获取比例系数
    float scale_input = static_cast<float>(dst_width) / src_width;  // 假设缩放比例在宽度和高度上相同
    float scale_infer = 1.0f / scale_input;
    
    // 生成从 input 到 infer 的仿射变换矩阵
    cv::Mat transform = cv::getRotationMatrix2D(dst_center, calculate_rotate_angle(src_center, dst_center), scale_infer);
    input_to_infer.matrix = transform;

    // 生成 
    transform = cv::getRotationMatrix2D(src_center, calculate_rotate_angle(dst_center, src_center), scale_input);

    infer_to_input.matrix = transform;
}