// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xmem_write.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XMem_write_CfgInitialize(XMem_write *InstancePtr, XMem_write_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XMem_write_Start(XMem_write *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_AP_CTRL) & 0x80;
    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XMem_write_IsDone(XMem_write *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XMem_write_IsIdle(XMem_write *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XMem_write_IsReady(XMem_write *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XMem_write_Continue(XMem_write *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_AP_CTRL) & 0x80;
    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_AP_CTRL, Data | 0x10);
}

void XMem_write_EnableAutoRestart(XMem_write *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XMem_write_DisableAutoRestart(XMem_write *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_AP_CTRL, 0);
}

void XMem_write_Set_mem(XMem_write *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_MEM_DATA, (u32)(Data));
    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_MEM_DATA + 4, (u32)(Data >> 32));
}

u64 XMem_write_Get_mem(XMem_write *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_MEM_DATA);
    Data += (u64)XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_MEM_DATA + 4) << 32;
    return Data;
}

void XMem_write_Set_size(XMem_write *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_SIZE_DATA, Data);
}

u32 XMem_write_Get_size(XMem_write *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_SIZE_DATA);
    return Data;
}

void XMem_write_InterruptGlobalEnable(XMem_write *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_GIE, 1);
}

void XMem_write_InterruptGlobalDisable(XMem_write *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_GIE, 0);
}

void XMem_write_InterruptEnable(XMem_write *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_IER);
    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_IER, Register | Mask);
}

void XMem_write_InterruptDisable(XMem_write *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_IER);
    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_IER, Register & (~Mask));
}

void XMem_write_InterruptClear(XMem_write *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMem_write_WriteReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_ISR, Mask);
}

u32 XMem_write_InterruptGetEnabled(XMem_write *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_IER);
}

u32 XMem_write_InterruptGetStatus(XMem_write *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMem_write_ReadReg(InstancePtr->Control_BaseAddress, XMEM_WRITE_CONTROL_ADDR_ISR);
}

