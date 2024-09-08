#ifndef __OPENRM_RESIZE_CUH__
#define __OPENRM_RESIZE_CUH__

#include "cudatools.h"

struct AffineMatrix {
    float value[6];
};

extern AffineMatrix input_to_infer;
extern AffineMatrix infer_to_input;

void generate_affine_matrix(int src_width, int src_height, int dst_width, int dst_height);

void warpaffine_kernel(
    uint8_t* src,
    int src_line_size,
    int src_width,
    int src_height,
    float* dst,
    int dst_width,
    int dst_height,
    uint8_t const_value_st,
    AffineMatrix d2s,
    int edge
);


#endif
