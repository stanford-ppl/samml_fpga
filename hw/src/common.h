#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <ap_int.h>

const int DATA_WIDTH = 17;
const int DONE_BIT = 7;
typedef ap_int<DATA_WIDTH> stream_type_t;

const stream_type_t default_stop = ap_int<17>(1) << 16;
const stream_type_t done_tkn = (ap_int<17>(1) << 16) | (ap_int<17>(1) << 7);

template<int N>
bool is_msb_set(ap_int<N> value) {
    return (value & (ap_int<N>(1) << (N - 1))) != 0;
}


#endif