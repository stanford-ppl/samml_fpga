// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.1 (64-bit)
// Tool Version Limit: 2024.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#ifdef SDT
#include "xparameters.h"
#endif
#include "xmem_read.h"

extern XMem_read_Config XMem_read_ConfigTable[];

#ifdef SDT
XMem_read_Config *XMem_read_LookupConfig(UINTPTR BaseAddress) {
	XMem_read_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XMem_read_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XMem_read_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XMem_read_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMem_read_Initialize(XMem_read *InstancePtr, UINTPTR BaseAddress) {
	XMem_read_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMem_read_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMem_read_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XMem_read_Config *XMem_read_LookupConfig(u16 DeviceId) {
	XMem_read_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XMEM_READ_NUM_INSTANCES; Index++) {
		if (XMem_read_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XMem_read_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMem_read_Initialize(XMem_read *InstancePtr, u16 DeviceId) {
	XMem_read_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMem_read_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMem_read_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

