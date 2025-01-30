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

#include "cmdlineparser.h"
#include <iostream>
#include <cstring>

// XRT includes
#include "experimental/xrt_bo.h"
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"

int main(int argc, char** argv) {
    // Command Line Parser
    sda::utils::CmdLineParser parser;

    // Switches
    //**************//"<Full Arg>",  "<Short Arg>", "<Description>", "<Default>"
    parser.addSwitch("--xclbin_file", "-x", "input binary file string", "");
    parser.addSwitch("--device_id", "-d", "device index", "0");
    parser.parse(argc, argv);

    // Read settings
    std::string binaryFile = parser.value("xclbin_file");
    int device_index = stoi(parser.value("device_id"));

    if (argc < 3) {
        parser.printHelp();
        return EXIT_FAILURE;
    }
    std::cout << "Open the device" << device_index << std::endl;
    auto device = xrt::device(device_index);
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);

    size_t data_size = 1024 * 1024;
    size_t vector_size_bytes = sizeof(int) * data_size;

    // Reducing the data size for emulation mode
    char* xcl_mode = getenv("XCL_EMULATION_MODE");
    if (xcl_mode != nullptr) {
        data_size = 1024;
    }

    // Allocate Memory in Host Memory
    std::vector<int> source_sw_results(data_size);

    auto mem_read = xrt::kernel(device, uuid, "mem_read");
    auto mem_write = xrt::kernel(device, uuid, "mem_write");

    auto buffer_input = xrt::bo(device, vector_size_bytes, mem_read.group_id(0));
    auto buffer_output = xrt::bo(device, vector_size_bytes, mem_write.group_id(0));

    auto source_input = buffer_input.map<int*>();
    auto source_hw_results = buffer_output.map<int*>();

    // Create the test data and Software Result
    for (size_t i = 0; i < data_size; i++) {
        source_input[i] = i;
        source_sw_results[i] = i * 10;
    }

    // Copy input data to device global memory
    std::cout << "Copying data..." << std::endl;
    buffer_input.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    // Launch the Kernel
    std::cout << "Launching Kernel..." << std::endl;
    int size = data_size;
    auto write_run = xrt::run(mem_write);
    write_run.set_arg(0, buffer_output);
    write_run.set_arg(1, size);
    write_run.start();

    auto read_run = xrt::run(mem_read);
    read_run.set_arg(0, buffer_input);
    read_run.set_arg(1, size);
    read_run.start();

    read_run.wait();
    write_run.wait();

    // Copy Result from Device Global Memory to Host Local Memory
    std::cout << "Getting Results..." << std::endl;
    buffer_output.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    // Compare the results of the Device to the simulation
    bool match = true;
    for (size_t i = 0; i < data_size; i++) {
        if (source_hw_results[i] != source_sw_results[i]) {
            std::cout << "Error: Result mismatch" << std::endl;
            std::cout << "i = " << i << " CPU result = " << source_sw_results[i]
                      << " Device result = " << source_hw_results[i] << std::endl;
            match = false;
            break;
        }
    }

    std::cout << "TEST " << (match ? "PASSED" : "FAILED") << std::endl;
    return (match ? EXIT_SUCCESS : EXIT_FAILURE);
}





// #include "cmdlineparser.h"
// #include <iostream>
// #include <cstring>

// // XRT includes
// #include "experimental/xrt_bo.h"
// #include "experimental/xrt_device.h"
// #include "experimental/xrt_kernel.h"
// // #include "common.h"
// #include "hls_stream.h"

// #define DATA_SIZE 4096
// #define DATA_WIDTH 17

// int main(int argc, char** argv) {
//     // Command Line Parser
//     sda::utils::CmdLineParser parser;

//     // Switches
//     //**************//"<Full Arg>",  "<Short Arg>", "<Description>", "<Default>"
//     parser.addSwitch("--xclbin_file", "-x", "input binary file string", "");
//     parser.addSwitch("--device_id", "-d", "device index", "0");
//     parser.parse(argc, argv);

//     // Read settings
//     std::string binaryFile = parser.value("xclbin_file");
//     int device_index = stoi(parser.value("device_id"));

//     if (argc < 3) {
//         parser.printHelp();
//         return EXIT_FAILURE;
//     }

//     std::cout << "Open the device" << device_index << std::endl;
//     auto device = xrt::device(device_index);
//     std::cout << "Load the xclbin " << binaryFile << std::endl;
//     auto uuid = device.load_xclbin(binaryFile);

//     size_t vector_size_bytes = sizeof(int) * DATA_SIZE;

//     auto krnl = xrt::kernel(device, uuid, "vadd");

//     std::cout << "Allocate Buffer in Global Memory\n";
//     // auto bo0 = xrt::bo(device, vector_size_bytes, krnl.group_id(0));
//     // auto bo1 = xrt::bo(device, vector_size_bytes, krnl.group_id(1));
//     // auto bo_out = xrt::bo(device, vector_size_bytes, krnl.group_id(2));

//     hls::stream<ap_int<DATA_WIDTH>> in_val, out_val;   

//     ap_int<DATA_WIDTH> test_data[] = {0, -1, -2, 3, 4, -10, 20, -30};
//     int num_values = sizeof(test_data) / sizeof(ap_int<DATA_WIDTH>);

//     for (int i = 0; i < num_values; i++) {
//         in_val.write(test_data[i]);
//     }


//     // Map the contents of the buffer object into host memory
//     // auto bo0_map = bo0.map<int*>();
//     // auto bo1_map = bo1.map<int*>();
//     // auto bo_out_map = bo_out.map<int*>();
//     // std::fill(bo0_map, bo0_map + DATA_SIZE, 1);
//     // std::fill(bo1_map, bo1_map + DATA_SIZE, 1);
//     // std::fill(bo_out_map, bo_out_map + DATA_SIZE, 1);

//     // Create the test data
//     // int bufReference[DATA_SIZE];
//     // for (int i = 0; i < DATA_SIZE; ++i) {
//     //     bo0_map[i] = i;
//     //     // bo1_map[i] = i;
//     //     // bufReference[i] = bo0_map[i] + bo1_map[i];
//     //     bufReference[i] = bo0_map[i];
//     // }

//     // Synchronize buffer content with device side
//     // std::cout << "synchronize input buffer data to device global memory\n";

//     // bo0.sync(XCL_BO_SYNC_BO_TO_DEVICE);
//     // bo1.sync(XCL_BO_SYNC_BO_TO_DEVICE);

//     std::cout << "Execution of the kernel\n";
//     auto run = krnl(in_val, out_val);
//     run.wait();

//     // Get the output;
//     std::cout << "Get the output data from the device" << std::endl;
//     // bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
//     td::cout << "Output values:\n";
//     while (!out_val.empty()) {
//         std::cout << out_val.read() << " ";
//     }
//     std::cout << std::endl;

//     // Validate our results
//     // if (std::memcmp(bo_out_map, bufReference, DATA_SIZE))
//         // throw std::runtime_error("Value read back does not match reference");

//     std::cout << "TEST PASSED\n";
//     return 0;
// }
