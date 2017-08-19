#pragma once

#include "bcb6_std_vector.h"
#include "THeapListData.h"
#define typename THeapListData
#include "bcb6_std_vector_template.h"

#ifdef USING_NAMESPACE_BCB6_STD
#define vector_THeapListData_push_back bcb6_std_vector_THeapListData_push_back
#endif

EXTERN_C void __fastcall bcb6_std_vector_THeapListData_push_back(bcb6_std_vector_THeapListData *heapList, THeapListData *heapListData);
