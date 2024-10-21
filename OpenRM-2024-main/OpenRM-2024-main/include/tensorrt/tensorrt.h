#ifndef __OPENRM_TENSORRT_TENSORRT_H__
#define __OPENRM_TENSORRT_TENSORRT_H__

#include <string>
#include <vector>
#include <openvino/openvino.hpp>
#include "structure/stamp.hpp"

namespace rm {
//初始化 OpenVINO ONNX 模型 (ONNX模型文件的路径/生成的引擎文件的路径/批处理大小)
bool initOpenVinoOnnx(
    const std::string& onnx_file,
    const std::string& engine_file,
    unsigned int batch_size = 1U
);

//初始化 OpenVINO 引擎
bool initOpenVinoEngine(
    const std::string& engine_file
);

//在推理请求中入队输入数据(infer_request OpenVINO的推理请求对象，用于管理推理过程。)
void detectEnqueue(
    float* input_data,
    float* output_data,
    ov::InferRequest& infer_request
);

//用于处理推理结果并将输出数据复制到主机缓冲区
void detectOutput(
    float* output_host_buffer,
    const float* output_device_buffer,
    size_t output_struct_size,
    int bboxes_num,
    int batch_size = 1
);

//为相机图像分配内存缓冲区
void mallocYoloCameraBuffer(
    uint8_t** rgb_host_buffer,
    uint8_t** rgb_device_buffer,
    int rgb_width,
    int rgb_height,
    int batch_size = 1,
    int channels = 3
);

//为 YOLO 检测分配内存缓冲区
void mallocYoloDetectBuffer(
    float** input_device_buffer,
    float** output_device_buffer,
    float** output_host_buffer,
    int input_width,
    int input_height,
    size_t output_struct_size,
    int bboxes_num,
    int batch_size = 1,
    int channels = 3
);

void freeYoloCameraBuffer(
    uint8_t* rgb_host_buffer,
    uint8_t* rgb_device_buffer
);

void freeYoloDetectBuffer(
    float* input_device_buffer,
    float* output_device_buffer,
    float* output_host_buffer
);

//用于将相机数据复制到缓冲区
void memcpyYoloCameraBuffer(
    uint8_t* rgb_mat_data,
    uint8_t* rgb_host_buffer,
    uint8_t* rgb_device_buffer,
    int rgb_width,
    int rgb_height,
    int channels = 3
);

void memcpyClassifyBuffer(
    uint8_t* mat_data,
    float* input_host_buffer,
    float* input_device_buffer,
    int input_width,
    int input_height,
    int channels = 3
);

std::vector<YoloRect> yoloArmorNMS_V5C36(
    float* output_host_buffer,
    int output_bboxes_num,
    int armor_classes_num,
    float confidence_threshold,
    float nms_threshold,
    int input_width,
    int input_height,
    int infer_width,
    int infer_height
);

std::vector<YoloRect> yoloArmorNMS_V5(
    float* output_host_buffer,
    int output_bboxes_num,
    int armor_classes_num,
    float confidence_threshold,
    float nms_threshold,
    int input_width,
    int input_height,
    int infer_width,
    int infer_height
);

std::vector<YoloRect> yoloArmorNMS_FP(
    float* output_host_buffer,
    int output_bboxes_num,
    int classes_num,
    float confidence_threshold,
    float nms_threshold,
    int input_width,
    int input_height,
    int infer_width,
    int infer_height
);

std::vector<YoloRect> yoloArmorNMS_FPX(
    float* output_host_buffer,
    int output_bboxes_num,
    int classes_num,
    float confidence_threshold,
    float nms_threshold,
    int input_width,
    int input_height,
    int infer_width,
    int infer_height
);

}

#endif
