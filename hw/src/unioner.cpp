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
#include "util.h"

#define DATA_SIZE 4096
#define empty 1 << 15

// TRIPCOUNT identifier
const int c_size = DATA_SIZE;


static void unioner(hls::stream<stream_type_t>& inCoordStream1,
                    hls::stream<stream_type_t>& inRefStream1,
                    hls::stream<stream_type_t>& inCoordStream2,
                    hls::stream<stream_type_t>& inRefStream2,
                    hls::stream<stream_type_t>& outCoordStream,
                    hls::stream<stream_type_t>& outRef1Stream,
                    hls::stream<stream_type_t>& outRef2Stream,
                    int size1, 
                    int size2) {

// Auto-pipeline is going to apply pipeline to this loop
execute:
    int size = (size1 > size2) ? size1 : size2;
    bool in1_wait = false;
    bool in2_wait = false;

    while (true) {
        stream_type_t coord_in1, ref_in1, coord_in2, ref_in2;
        if (size1 >= i & !in1_wait) {
            inCoordStream1.read(coord_in1);
            inRefStream1.read(ref_in1);
        }
        if (size2 >= i & !in2_wait) {
            inCoordStream2.read(coord_in2);
            inRefStream2.read(ref_in2);
        }
        in1_wait = false;
        in2_wait = false;

        if (is_done(coord_in1) & is_done(coord_in2)) {
            outRef1Stream << ref_in1;
            outRef2Stream << ref_in1;
            outCoordStream << coord_in1;
            break;
        }
        else if (is_stop(coord_in1) & !is_stop(coord_in2)) {
            outRef2Stream << ref_in2;
            outCoordStream << coord_in2;
            in1_wait = true;
        }
        else if (!is_stop(coord_in1) & is_stop(coord_in2)) {
            outRef1Stream << ref_in1;
            outCoordStream << coord_in1;
            in2_wait = true;
        }
        else if (is_stop(coord_in1) & is_stop(coord_in2)) {
            if (coord_in1 != coord_in2) {
                panic("stop token mismatch\n");
            }
            else {
                outRef1Stream << ref_in1;
                outRef2Stream << ref_in2;
                outCoordStream << coord_in1;
            }
        }
        else if (is_data(coord_in1) & is_data(coord_in2)) {
            if (coord_in1 == coord_in2) {
                outRef1Stream << ref_in1;
                outRef2Stream << ref_in2;
                outCoordStream << coord_in1;
            }
            else if (coord_in1 > coord_in2) {
                outRef1Stream << empty;
                outRef2Stream << ref_in2;
                outCoordStream << coord_in2;
                in1_wait = true;
            }
            else if (coord_in2 > coord_in1) {
                outRef2Stream << empty;
                outRef1Stream << ref_in1;
                outCoordStream << coord_in1;
                in2_wait = true;
            }
        }
        else {
            panic("Token mismatch\n");
        }
    }
}

extern "C" {
/*
    Vector Addition Kernel Implementation using dataflow
    Arguments:
        in1   (input)  --> Input Vector 1
        in2   (input)  --> Input Vector 2
        out  (output) --> Output Vector
        size (input)  --> Size of Vector in Integer
   */
void unioner(
    stream_type_t* in1, stream_type_t* in2, stream_type_t* out, int size) {
    static hls::stream<stream_type_t> inStream1("input_stream_1");
    static hls::stream<stream_type_t> inStream2("input_stream_2");
    static hls::stream<stream_type_t> outStream("output_stream");

#pragma HLS INTERFACE m_axi port = in1 bundle = gmem0
#pragma HLS INTERFACE m_axi port = in2 bundle = gmem1
#pragma HLS INTERFACE m_axi port = out bundle = gmem0

#pragma HLS dataflow
    // dataflow pragma instruct compiler to run following three APIs in parallel
    read_input(in1, inStream1, size);
    read_input(in2, inStream2, size);
    // compute_add(inStream1, inStream2, outStream, size);
    alu(inStream1, inStream2, outStream, size, 0);
    write_result(out, outStream, size);
}
}
