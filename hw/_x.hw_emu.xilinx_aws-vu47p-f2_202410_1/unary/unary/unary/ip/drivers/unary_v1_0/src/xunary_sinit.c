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
#include "xunary.h"

extern XUnary_Config XUnary_ConfigTable[];

#ifdef SDT
XUnary_Config *XUnary_LookupConfig(UINTPTR BaseAddress) {
	XUnary_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XUnary_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XUnary_ConfigTable[Index].Control_BaseAddress == BaseAddress) {
			ConfigPtr = &XUnary_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XUnary_Initialize(XUnary *InstancePtr, UINTPTR BaseAddress) {
	XUnary_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XUnary_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XUnary_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XUnary_Config *XUnary_LookupConfig(u16 DeviceId) {
	XUnary_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XUNARY_NUM_INSTANCES; Index++) {
		if (XUnary_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XUnary_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XUnary_Initialize(XUnary *InstancePtr, u16 DeviceId) {
	XUnary_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XUnary_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XUnary_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

