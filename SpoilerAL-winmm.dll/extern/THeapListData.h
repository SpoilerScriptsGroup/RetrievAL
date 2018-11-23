#pragma once

#include <windows.h>
#include "tlhelp32fix.h"

typedef struct
{
	HEAPLIST32 heapList;
	DWORD      heapListAddress;
	DWORD      heapListSize;
} THeapListData;

#define typename THeapListData
#include "bcb6_std_vector_template.h"

