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
#include "xmem_write.h"

extern XMem_write_Config XMem_write_ConfigTable[];

#ifdef SDT
XMem_write_Config *XMem_write_LookupConfig(UINTPTR BaseAddress) {
	XMem_write_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XMem_write_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XMem_write_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XMem_write_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMem_write_Initialize(XMem_write *InstancePtr, UINTPTR BaseAddress) {
	XMem_write_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMem_write_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMem_write_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XMem_write_Config *XMem_write_LookupConfig(u16 DeviceId) {
	XMem_write_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XMEM_WRITE_NUM_INSTANCES; Index++) {
		if (XMem_write_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XMem_write_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMem_write_Initialize(XMem_write *InstancePtr, u16 DeviceId) {
	XMem_write_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMem_write_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMem_write_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

