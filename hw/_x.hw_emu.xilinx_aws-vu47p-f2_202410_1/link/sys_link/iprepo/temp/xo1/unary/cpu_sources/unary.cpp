/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

/*******************************************************************************
Description:
    This example uses the load/compute/store coding style, which is generally
    the most efficient for implementing kernels using HLS. The load and store
    functions are responsible for moving data in and out of the kernel as
    efficiently as possible. The core functionality is decomposed across one
    of more compute functions. Whenever possible, the compute function should
    pass data through HLS streams and should contain a single set of nested loops.
    HLS stream objects are used to pass data between producer and consumer
    functions. Stream read and write operations have a blocking behavior which
    allows consumers and producers to synchronize with each other automatically.
    The dataflow pragma instructs the compiler to enable task-level pipelining.
    This is required for to load/compute/store functions to execute in a parallel
    and pipelined manner.
    The kernel operates on vectors of NUM_WORDS integers modeled using the hls::vector
    data type. This datatype provides intuitive support for parallelism and
    fits well the vector-add computation. The vector length is set to NUM_WORDS
    since NUM_WORDS integers amount to a total of 64 bytes, which is the maximum size of
    a kernel port. It is a good practice to match the compute bandwidth to the I/O
    bandwidth. Here the kernel loads, computes and stores NUM_WORDS integer values per
    clock cycle and is implemented as below:
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
                                      | compute_add |      __
                                      |_____________|---->|  |
                                       ______________     |  | out_stream
                                      |              |<---|__|
                                      | store_result |
                                      |______________|-----> Output result to Global Memory

*******************************************************************************/

#include <stdint.h>
#include <hls_stream.h>
#include "common.h"

#define DATA_SIZE 4096

enum UnaryOp {
    RELU = 0,
    SCALAR_MUL = 1
};

// Function to compute the unary operation
ap_int<DATA_WIDTH> unary_compute(stream_type_t val) {
#pragma HLS INLINE
    if (is_msb_set(val)) {
        return val;
    } else {
        return val * 10;
    }
}

// extern "C" {
// Streaming Unary ALU function
void unary(hls::stream<stream_type_t> &in_val,
               hls::stream<stream_type_t> &out_val) {
#pragma HLS INTERFACE axis port=in_val
#pragma HLS INTERFACE axis port=out_val
// #pragma HLS INTERFACE s_axilite port=op bundle=control
// #pragma HLS INTERFACE s_axilite port=scalar bundle=control
// #pragma HLS INTERFACE s_axilite port=return bundle=control

    while (!in_val.empty()) {
#pragma HLS PIPELINE II=1
        ap_int<DATA_WIDTH> in_data = in_val.read();
        ap_int<DATA_WIDTH> result = unary_compute(in_data);
        out_val.write(result);
    }
}

// }
