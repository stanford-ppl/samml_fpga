// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xunary.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XUnary_CfgInitialize(XUnary *InstancePtr, XUnary_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XUnary_Start(XUnary *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUnary_ReadReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_AP_CTRL) & 0x80;
    XUnary_WriteReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XUnary_IsDone(XUnary *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUnary_ReadReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XUnary_IsIdle(XUnary *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUnary_ReadReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XUnary_IsReady(XUnary *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUnary_ReadReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XUnary_Continue(XUnary *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XUnary_ReadReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_AP_CTRL) & 0x80;
    XUnary_WriteReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_AP_CTRL, Data | 0x10);
}

void XUnary_EnableAutoRestart(XUnary *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUnary_WriteReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XUnary_DisableAutoRestart(XUnary *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUnary_WriteReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_AP_CTRL, 0);
}

void XUnary_InterruptGlobalEnable(XUnary *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUnary_WriteReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_GIE, 1);
}

void XUnary_InterruptGlobalDisable(XUnary *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUnary_WriteReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_GIE, 0);
}

void XUnary_InterruptEnable(XUnary *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XUnary_ReadReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_IER);
    XUnary_WriteReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_IER, Register | Mask);
}

void XUnary_InterruptDisable(XUnary *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XUnary_ReadReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_IER);
    XUnary_WriteReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_IER, Register & (~Mask));
}

void XUnary_InterruptClear(XUnary *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XUnary_WriteReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_ISR, Mask);
}

u32 XUnary_InterruptGetEnabled(XUnary *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XUnary_ReadReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_IER);
}

u32 XUnary_InterruptGetStatus(XUnary *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XUnary_ReadReg(InstancePtr->Control_BaseAddress, XUNARY_CONTROL_ADDR_ISR);
}

