// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XUNARY_H
#define XUNARY_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xunary_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
#ifdef SDT
    char *Name;
#else
    u16 DeviceId;
#endif
    u64 Control_BaseAddress;
} XUnary_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XUnary;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XUnary_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XUnary_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XUnary_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XUnary_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
#ifdef SDT
int XUnary_Initialize(XUnary *InstancePtr, UINTPTR BaseAddress);
XUnary_Config* XUnary_LookupConfig(UINTPTR BaseAddress);
#else
int XUnary_Initialize(XUnary *InstancePtr, u16 DeviceId);
XUnary_Config* XUnary_LookupConfig(u16 DeviceId);
#endif
int XUnary_CfgInitialize(XUnary *InstancePtr, XUnary_Config *ConfigPtr);
#else
int XUnary_Initialize(XUnary *InstancePtr, const char* InstanceName);
int XUnary_Release(XUnary *InstancePtr);
#endif

void XUnary_Start(XUnary *InstancePtr);
u32 XUnary_IsDone(XUnary *InstancePtr);
u32 XUnary_IsIdle(XUnary *InstancePtr);
u32 XUnary_IsReady(XUnary *InstancePtr);
void XUnary_Continue(XUnary *InstancePtr);
void XUnary_EnableAutoRestart(XUnary *InstancePtr);
void XUnary_DisableAutoRestart(XUnary *InstancePtr);


void XUnary_InterruptGlobalEnable(XUnary *InstancePtr);
void XUnary_InterruptGlobalDisable(XUnary *InstancePtr);
void XUnary_InterruptEnable(XUnary *InstancePtr, u32 Mask);
void XUnary_InterruptDisable(XUnary *InstancePtr, u32 Mask);
void XUnary_InterruptClear(XUnary *InstancePtr, u32 Mask);
u32 XUnary_InterruptGetEnabled(XUnary *InstancePtr);
u32 XUnary_InterruptGetStatus(XUnary *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
