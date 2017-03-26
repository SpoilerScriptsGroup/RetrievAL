#pragma once

#include <windows.h>
#include "tlhelp32fix.h"

typedef struct
{
	HEAPLIST32 heapList;
	DWORD      heapListAddress;
	DWORD      heapListSize;
} THeapListData;

