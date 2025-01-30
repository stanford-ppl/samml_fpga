// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XMEM_WRITE_H
#define XMEM_WRITE_H

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
#include "xmem_write_hw.h"

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
} XMem_write_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XMem_write;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XMem_write_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XMem_write_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XMem_write_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XMem_write_ReadReg(BaseAddress, RegOffset) \
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
int XMem_write_Initialize(XMem_write *InstancePtr, UINTPTR BaseAddress);
XMem_write_Config* XMem_write_LookupConfig(UINTPTR BaseAddress);
#else
int XMem_write_Initialize(XMem_write *InstancePtr, u16 DeviceId);
XMem_write_Config* XMem_write_LookupConfig(u16 DeviceId);
#endif
int XMem_write_CfgInitialize(XMem_write *InstancePtr, XMem_write_Config *ConfigPtr);
#else
int XMem_write_Initialize(XMem_write *InstancePtr, const char* InstanceName);
int XMem_write_Release(XMem_write *InstancePtr);
#endif

void XMem_write_Start(XMem_write *InstancePtr);
u32 XMem_write_IsDone(XMem_write *InstancePtr);
u32 XMem_write_IsIdle(XMem_write *InstancePtr);
u32 XMem_write_IsReady(XMem_write *InstancePtr);
void XMem_write_Continue(XMem_write *InstancePtr);
void XMem_write_EnableAutoRestart(XMem_write *InstancePtr);
void XMem_write_DisableAutoRestart(XMem_write *InstancePtr);

void XMem_write_Set_mem(XMem_write *InstancePtr, u64 Data);
u64 XMem_write_Get_mem(XMem_write *InstancePtr);
void XMem_write_Set_size(XMem_write *InstancePtr, u32 Data);
u32 XMem_write_Get_size(XMem_write *InstancePtr);

void XMem_write_InterruptGlobalEnable(XMem_write *InstancePtr);
void XMem_write_InterruptGlobalDisable(XMem_write *InstancePtr);
void XMem_write_InterruptEnable(XMem_write *InstancePtr, u32 Mask);
void XMem_write_InterruptDisable(XMem_write *InstancePtr, u32 Mask);
void XMem_write_InterruptClear(XMem_write *InstancePtr, u32 Mask);
u32 XMem_write_InterruptGetEnabled(XMem_write *InstancePtr);
u32 XMem_write_InterruptGetStatus(XMem_write *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
