#include "resize.h"

//void rm::resize(
//    uint8_t* src,
//    int src_width,
//    int src_height,
//    float* dst,
//    int dst_width,
//    int dst_height,
//    void* _cuda_stream
//) {
    //void* _cuda_stream
    //// 生成仿射变换矩阵
    //generate_affine_matrix(src_width, src_height, dst_width, dst_height);

    //// 计算线程块和线程数量
    //int jobs = dst_width * dst_height;
    //int threads = 256;
    //int blocks = ceil(jobs / (float)threads);

    //// 启动核函数
    //warpaffine_kernel<<<blocks, threads, 0, cuda_stream>>>(
    //    src,
    //    src_width * 3,
    //    src_width,
    //    src_height,
    //    dst,
    //    dst_width,
    //    dst_height,
    //    114,
    //    infer_to_input,
    //    jobs
    //);

    //// 等待核函数执行完成
    //cudaStreamSynchronize(cuda_stream);
//}

void rm::resize(const int &src, int &dst, int dst_width, int dst_height)
{
    // 生成仿射变换矩阵
    generate_affine_matrix(src.cols, src.rows, dst_width, dst_height);

    // 执行仿射变换
    cv::Mat temp;
    warpaffine_kernel(src, temp, input_to_infer);

    // 如果目标尺寸与源尺寸不同，则进行缩放
    if(dst_width != src.cols || dst_height != src.rows) {
        cv::resize(temp, dst, cv::Size(dst_width, dst_height), 0, 0, cv::INTER_LINEAR);
    } else {
        temp.copyTo(dst);
    }
}

