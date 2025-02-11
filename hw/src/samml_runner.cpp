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
    if (is_stkn(val)) {
        return val;
    } else {
        return val * 10;
    }
}

// Generator for initial root node
void generator(hls::stream<stream_type_t> &out_ref) {
    out_ref.write(0);
    out_ref.write(done_tkn);
}

// Read scanner implementation
void rd_scanner(unsigned int* seg, unsigned int* crd, hls::stream<stream_type_t> &in_ref, hls::stream<stream_type_t> &out_crd, hls::stream<stream_type_t> &out_ref) {
    #pragma HLS interface ap_ctrl_none port = return
    stream_type_t curr_ref = in_ref.read();
    if(is_stkn(curr_ref)) {
        out_crd.write(default_stop || (curr_ref + 1));
        out_ref.write(default_stop || (curr_ref + 1));
    } else if(is_dtkn(curr_ref)) {
        out_crd.write(done_tkn);
        out_ref.write(done_tkn);
    } else {
        unsigned int curr_addr = seg[curr_ref];
        unsigned int stop_addr = seg[curr_ref + 1];

        for (unsigned int i = curr_addr; i < stop_addr; i++) {
            out_crd.write(crd[i]);
            out_ref.write(i);
        }
        // FIXME: Need to figure out how to peek at the next element without popping it from the stream
        stream_type_t next_tkn;
        out_crd.write(default_stop);
        out_ref.write(default_stop);
    }
}

// Write scanner implementation
void wr_scanner(unsigned int* seg, unsigned int* crd, hls::stream<stream_type_t> &in_crd) {
    #pragma HLS interface ap_ctrl_none port = return
    stream_type_t curr_val = in_crd.read();
    stream_type_t curr_crd_cnt = 0;

    bool end_fiber = false;
    if(is_stkn(curr_val) && !end_fiber) {
        // seg.write(curr_crd_cnt);
        seg[curr_crd_cnt] = curr_crd_cnt;
        end_fiber = true;
    } else if (is_dtkn(curr_val)) {
        return;
    } else {
        crd[curr_crd_cnt] = curr_val;
        curr_crd_cnt += 1;
        end_fiber = false;
    }
}

void valwr_scanner(unsigned int* val, hls::stream<stream_type_t> &in_val) {
    #pragma HLS interface ap_ctrl_none port = return

    stream_type_t curr_val = in_val.read();
    unsigned int counter = 0;

    if (!is_stkn(curr_val) && !is_dtkn(curr_val)) {
        val[counter++] = curr_val;
    }
}

// Array implementation
void array(unsigned int* mem, hls::stream<stream_type_t> &in_ref, hls::stream<stream_type_t> &out_val) {
    #pragma HLS interface ap_ctrl_none port = return
    stream_type_t curr_ref = in_ref.read();

    if(is_stkn(curr_ref) || is_dtkn(curr_ref)){
        out_val.write(curr_ref);
    } else {
        out_val.write(mem[curr_ref]);
    }
}

static void alu(hls::stream<stream_type_t> &inStream1,
                hls::stream<stream_type_t> &inStream2,
                hls::stream<stream_type_t> &outStream,
                int op) {
// op: 
// 0: add
// 1: mul
    #pragma HLS interface ap_ctrl_none port = return
    stream_type_t in1 = inStream1.read();
    stream_type_t in2 = inStream2.read();

    if (is_stkn(in1) & is_stkn(in2)) {
        if (in1 == in2) {
            outStream.write(in1);
        } else {
            panic("Stop token mismatch\n");
        }
    } else if (is_dtkn(in1) & is_dtkn(in2)) {
        outStream.write(in1);
    } else if (!is_stkn(in1) & !is_dtkn(in1) & !is_stkn(in2) & !is_dtkn(in2)) {
        if (op == 0) {
            outStream.write(in1 + in2);
        } else if (op == 1) {
            outStream.write(in1 * in2);
        }
    } else {
        panic("Token mismatch\n");
    }
}

static void unioner(hls::stream<stream_type_t> &inCoordStream1,
                    hls::stream<stream_type_t> &inRefStream1,
                    hls::stream<stream_type_t> &inCoordStream2,
                    hls::stream<stream_type_t> &inRefStream2,
                    hls::stream<stream_type_t> &outCoordStream,
                    hls::stream<stream_type_t> &outRef1Stream,
                    hls::stream<stream_type_t> &outRef2Stream) {

    #pragma HLS interface ap_ctrl_none port = return
    bool in1_wait = false;
    bool in2_wait = false;

    stream_type_t coord_in1, ref_in1, coord_in2, ref_in2;
    if (!in1_wait) {
        coord_in1 = inCoordStream1.read();
        ref_in1 = inRefStream1.read();
    }
    if (!in2_wait) {
        coord_in2 = inCoordStream2.read();
        ref_in2 = inRefStream2.read();
    }

    if (is_dtkn(coord_in1) & is_dtkn(coord_in2)) {
        outRef1Stream.write(ref_in1);
        outRef2Stream.write(ref_in2);
        outCoordStream.write(coord_in1);
        break;
    } else if (is_stkn(coord_in1) & !is_stkn(coord_in2)) {
        outRef2Stream.write(ref_in2);
        outCoordStream.write(coord_in2);
        in1_wait = true;
        in2_wait = false;
    } else if (!is_stkn(coord_in1) & is_stkn(coord_in2)) {
        outRef1Stream.write(ref_in1);
        outCoordStream.write(coord_in1);
        in1_wait = false;
        in2_wait = true;
    } else if (is_stkn(coord_in1) & is_stkn(coord_in2)) {
        if (coord_in1 != coord_in2) {
            panic("stop token mismatch\n");
        } else {
            outRef1Stream.write(ref_in1);
            outRef2Stream.write(ref_in2);
            outCoordStream.write(coord_in1);
            in1_wait = false;
            in2_wait = false;
        }
    } else if (!is_dtkn(coord_in1) & !is_stkn(coord_in1) & !is_dtkn(coord_in2) & !is_stkn(coord_in2)) {
        if (coord_in1 == coord_in2) {
            outRef1Stream.write(ref_in1);
            outRef2Stream.write(ref_in2);
            outCoordStream.write(coord_in1);
            in1_wait = false;
            in2_wait = false;
        } else if (coord_in1 > coord_in2) {
            outRef1Stream.write(EMPTY);
            outRef2Stream.write(ref_in2);
            outCoordStream.write(coord_in2);
            in1_wait = true;
            in2_wait = false;
        } else if (coord_in2 > coord_in1) {
            outRef2Stream.write(EMPTY);
            outRef1Stream.write(ref_in1);
            outCoordStream.write(coord_in1);
            in1_wait = false;
            in2_wait = true;
        }
    } else {
        panic("Token mismatch\n");
    }
    
}


static void intersecter(hls::stream<stream_type_t> &inCoordStream1,
                        hls::stream<stream_type_t> &inRefStream1,
                        hls::stream<stream_type_t> &inCoordStream2,
                        hls::stream<stream_type_t> &inRefStream2,
                        hls::stream<stream_type_t> &outCoordStream,
                        hls::stream<stream_type_t> &outRef1Stream,
                        hls::stream<stream_type_t> &outRef2Stream) {

    #pragma HLS interface ap_ctrl_none port = return
    bool in1_wait = false;
    bool in2_wait = false;

    stream_type_t coord_in1, ref_in1, coord_in2, ref_in2;
    if (!in1_wait) {
        coord_in1 = inCoordStream1.read();
        ref_in1 = inRefStream1.read();
    }
    if (!in2_wait) {
        coord_in2 = inCoordStream2.read();
        ref_in2 = inRefStream2.read();
    }

    if (is_dtkn(coord_in1) & is_dtkn(coord_in2)) {
        outRef1Stream.write(ref_in1);
        outRef2Stream.write(ref_in2);
        outCoordStream.write(coord_in1);
        break;
    } else if (is_stkn(coord_in1) & !is_stkn(coord_in2)) {
        in1_wait = true;
        in2_wait = false;
    } else if (!is_stkn(coord_in1) & is_stkn(coord_in2)) {
        in1_wait = false;
        in2_wait = true;
    } else if (is_stkn(coord_in1) & is_stkn(coord_in2)) {
        if (coord_in1 != coord_in2) {
            panic("stop token mismatch\n");
        } else {
            outRef1Stream.write(ref_in1);
            outRef2Stream.write(ref_in2);
            outCoordStream.write(coord_in1);
            in1_wait = false;
            in2_wait = false;
        }
    } else if (!is_dtkn(coord_in1) & !is_stkn(coord_in1) & !is_dtkn(coord_in2) & !is_stkn(coord_in2)) {
        if (coord_in1 == coord_in2) {
            outRef1Stream.write(ref_in1);
            outRef2Stream.write(ref_in2);
            outCoordStream.write(coord_in1);
            in1_wait = false;
            in2_wait = false;
        } else if (coord_in1 > coord_in2) {
            in1_wait = true;
            in2_wait = false;
        } else if (coord_in2 > coord_in1) {
            in1_wait = false;
            in2_wait = true;
        }
    } else {
        panic("Token mismatch\n");
    }
    

// Streaming Unary ALU function
void unary(hls::stream<stream_type_t> &in_val,
               hls::stream<stream_type_t> &out_val) {
// This line is needed for free running kernel!
#pragma HLS interface ap_ctrl_none port = return

        ap_int<DATA_WIDTH> in_data = in_val.read();
        ap_int<DATA_WIDTH> result = unary_compute(in_data);
        out_val.write(result);
}

extern "C" {

void samml_runner(unsigned int* segs, unsigned int* crds, unsigned int* vals, unsigned int* out_segs, unsigned int* out_crds, unsigned int* out_vals) {
    static hls::stream<stream_type_t> in_ref("in_ref");
    static hls::stream<stream_type_t> in_crd("in_crd");
    static hls::stream<stream_type_t> in_val("in_val");
    static hls::stream<stream_type_t> out_ref("out_ref");
    static hls::stream<stream_type_t> out_crd("out_crd");
    static hls::stream<stream_type_t> out_val("out_val");

    #pragma HLS INTERFACE m_axi port = segs bundle = gmem0
    #pragma HLS INTERFACE m_axi port = crds bundle = gmem1
    #pragma HLS INTERFACE m_axi port = vals bundle = gmem2
    #pragma HLS INTERFACE m_axi port = out_segs bundle = gmem0
    #pragma HLS INTERFACE m_axi port = out_crds bundle = gmem1
    #pragma HLS INTERFACE m_axi port = out_vals bundle = gmem2

    #pragma HLS dataflow
    generator(in_ref);
    rd_scanner(segs, crds, in_ref, out_crd, out_ref);
    array(vals, out_ref, in_val);
    unary(in_val, out_val);
    wr_scanner(out_segs, out_crds, out_crd);
    valwr_scanner(out_vals, out_val);
}

}
