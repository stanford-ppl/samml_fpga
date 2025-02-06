#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <ap_int.h>

const int DATA_WIDTH = 17;
typedef ap_int<DATA_WIDTH> stream_type_t;

const stream_type_t default_stop = ap_int<17>(1) << 16;
const stream_type_t done_tkn = (ap_int<17>(1) << 16) | (ap_int<17>(1) << 7);

template<int N>
bool is_stkn(ap_int<N> value) {
    return (value & (ap_int<N>(1) << (N - 1))) != 0;
}

bool is_dtkn(stream_type_t tkn) {
    return tkn == done_tkn;
}

// class tensor_data {
// private:
//     tensor_data(std::string name, int *shape, int **segs, int **crds, float *values) : name(name), shape(shape), segs(segs), crds(crds), values(values) {}
// public:
//     std::string name;
//     int *shape;
//     int **segs;
//     int **crds;
//     float *values;
// };


#endif