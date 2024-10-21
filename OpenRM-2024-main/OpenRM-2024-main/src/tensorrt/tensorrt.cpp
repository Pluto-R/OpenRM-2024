#include "tensorrt/tensorrt.h"
#include "uniterm/uniterm.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <openvino/openvino.hpp>

bool rm::initOpenVinoOnnx(
    const std::string& onnx_file,
    const std::string& engine_file,
    unsigned int batch_size
) {
    try {
        // 检查 ONNX 文件是否存在
        if (access(onnx_file.c_str(), F_OK) != 0) {
            throw std::runtime_error("ONNX file not found.");
        }

        // 创建 OpenVINO 核心对象
        ov::Core core;

        // 加载 ONNX 模型
        std::shared_ptr<ov::Model> model = core.read_model(onnx_file);

        // 编译模型以使用 CPU 进行推理
        ov::CompiledModel compiled_model = core.compile_model(model, "CPU");

        // 保存编译后的模型
        std::ofstream file(engine_file, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open engine file for writing.");
        }
        file.close();

        rm::message("OpenVINO ONNX model parsed and engine built", rm::MSG_OK);
        return true;
    } catch (const std::exception& e) {
        std::string error_message = e.what();
        rm::message("OpenVINO ONNX : " + error_message, rm::MSG_ERROR);
        return false;
    }
}

bool rm::initOpenVinoEngine(
    const std::string& engine_file
) {
    try {
        // 检查引擎文件是否存在
        if(access(engine_file.c_str(), F_OK) != 0) {
            throw std::runtime_error("Engine file not found.");
        }

        // 使用 OpenVINO 核心对象加载引擎
        ov::Core core;
        std::shared_ptr<ov::Model> model = core.read_model(engine_file);

        // 编译模型使用 CPU
        ov::CompiledModel compiled_model = core.compile_model(model, "CPU");

        rm::message("OpenVINO Engine OK", rm::MSG_OK);
        return true;
    } catch (const std::exception& e) {
        std::string error_message = e.what();
        rm::message("OpenVINO Engine : " + error_message, rm::MSG_ERROR);
        return false;
    }
}

void rm::detectEnqueue(
    float* input_data,
    float* output_data,
    ov::InferRequest& infer_request
) {
    try {
        // 准备输入数据
        ov::Tensor input_tensor = infer_request.get_input_tensor();
        std::memcpy(input_tensor.data<float>(), input_data, input_tensor.get_byte_size());

        // 执行推理
        infer_request.infer();

        // 获取输出数据
        ov::Tensor output_tensor = infer_request.get_output_tensor();
        std::memcpy(output_data, output_tensor.data<float>(), output_tensor.get_byte_size());

        rm::message("Inference completed", rm::MSG_OK);
    } catch (const std::exception& e) {
        std::string error_message = e.what();
        rm::message("Inference Error: " + error_message, rm::MSG_ERROR);
    }
}

void rm::mallocYoloDetectBuffer(
    float** input_device_buffer,
    float** output_device_buffer,
    float** output_host_buffer,
    int input_width,
    int input_height,
    size_t output_struct_size,
    int bboxes_num,
    int batch_size,
    int channels
) {
    *input_device_buffer = static_cast<float*>(malloc(input_width * input_height * channels * batch_size * sizeof(float)));
    *output_device_buffer = static_cast<float*>(malloc((output_struct_size * bboxes_num + 1) * batch_size * sizeof(float)));
    *output_host_buffer = static_cast<float*>(malloc((output_struct_size * bboxes_num + 1) * batch_size * sizeof(float)));
    rm::message("Yolo Detect Buffer allocated", rm::MSG_OK);
}

void rm::freeYoloDetectBuffer(
    float* input_device_buffer,
    float* output_device_buffer,
    float* output_host_buffer
) {
    free(input_device_buffer);
    free(output_device_buffer);
    free(output_host_buffer);
    rm::message("Yolo Detect Buffer freed", rm::MSG_WARNING);
}

void rm::memcpyYoloCameraBuffer(
    uint8_t* rgb_mat_data,
    uint8_t* rgb_host_buffer,
    uint8_t* rgb_device_buffer,
    int rgb_width,
    int rgb_height,
    int channels
) {
    size_t rgb_size = rgb_width * rgb_height * channels * sizeof(uint8_t);
    memcpy(rgb_host_buffer, rgb_mat_data, rgb_size);
}
