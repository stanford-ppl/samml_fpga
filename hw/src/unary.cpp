/*******************************************************************************
Description:
    This example implements a simple unary ALU. It process an input stream and performs some unary operation and then outputs a stream of the results
                                       _____________
                                      |             |<----- Input Vector 1 from Global Memory
                                      |  load_input |       __
                                      |_____________|----->|  |
                                       _____________       |  | in1_stream
Input Vector 2 from Global Memory --->|             |      |__|
                               __     |  load_input |        |
                              |  |<---|_____________|        |
                   in2_stream |  |     _____________         |
                              |__|--->|             |<--------
                                      | unary       |      __
                                      |_____________|---->|  |
                                       ______________     |  | out_stream
                                      |              |<---|__|
                                      | store_result |
                                      |______________|-----> Output result to Global Memory

*******************************************************************************/

#include <stdint.h>
#include <hls_stream.h>
#include "common.h"

enum UnaryOp {
    RELU = 0,
    SCALAR_MUL = 1
};

// Function to compute the unary operation
ap_int<DATA_WIDTH> unary_compute(stream_type_t val) {
    if (is_msb_set(val)) {
        return val;
    } else {
        return val * 10;
    }
}

extern "C" {
// Streaming Unary ALU function
void unary(hls::stream<stream_type_t> &in_val,
               hls::stream<stream_type_t> &out_val) {
// This line is needed for free running kernel!
#pragma HLS interface ap_ctrl_none port = return

        ap_int<DATA_WIDTH> in_data = in_val.read();
        ap_int<DATA_WIDTH> result = unary_compute(in_data);
        out_val.write(result);
}

}
