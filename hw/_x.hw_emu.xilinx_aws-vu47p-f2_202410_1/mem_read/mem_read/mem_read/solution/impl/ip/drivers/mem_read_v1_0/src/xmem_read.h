// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XMEM_READ_H
#define XMEM_READ_H

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
#include "xmem_read_hw.h"

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
} XMem_read_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XMem_read;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XMem_read_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XMem_read_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XMem_read_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XMem_read_ReadReg(BaseAddress, RegOffset) \
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
int XMem_read_Initialize(XMem_read *InstancePtr, UINTPTR BaseAddress);
XMem_read_Config* XMem_read_LookupConfig(UINTPTR BaseAddress);
#else
int XMem_read_Initialize(XMem_read *InstancePtr, u16 DeviceId);
XMem_read_Config* XMem_read_LookupConfig(u16 DeviceId);
#endif
int XMem_read_CfgInitialize(XMem_read *InstancePtr, XMem_read_Config *ConfigPtr);
#else
int XMem_read_Initialize(XMem_read *InstancePtr, const char* InstanceName);
int XMem_read_Release(XMem_read *InstancePtr);
#endif

void XMem_read_Start(XMem_read *InstancePtr);
u32 XMem_read_IsDone(XMem_read *InstancePtr);
u32 XMem_read_IsIdle(XMem_read *InstancePtr);
u32 XMem_read_IsReady(XMem_read *InstancePtr);
void XMem_read_Continue(XMem_read *InstancePtr);
void XMem_read_EnableAutoRestart(XMem_read *InstancePtr);
void XMem_read_DisableAutoRestart(XMem_read *InstancePtr);

void XMem_read_Set_mem(XMem_read *InstancePtr, u64 Data);
u64 XMem_read_Get_mem(XMem_read *InstancePtr);
void XMem_read_Set_size(XMem_read *InstancePtr, u32 Data);
u32 XMem_read_Get_size(XMem_read *InstancePtr);

void XMem_read_InterruptGlobalEnable(XMem_read *InstancePtr);
void XMem_read_InterruptGlobalDisable(XMem_read *InstancePtr);
void XMem_read_InterruptEnable(XMem_read *InstancePtr, u32 Mask);
void XMem_read_InterruptDisable(XMem_read *InstancePtr, u32 Mask);
void XMem_read_InterruptClear(XMem_read *InstancePtr, u32 Mask);
u32 XMem_read_InterruptGetEnabled(XMem_read *InstancePtr);
u32 XMem_read_InterruptGetStatus(XMem_read *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
