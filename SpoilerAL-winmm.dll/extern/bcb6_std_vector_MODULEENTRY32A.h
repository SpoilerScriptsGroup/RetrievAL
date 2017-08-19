#pragma once

#include "bcb6_std_vector.h"
#include "tlhelp32fix.h"
#define typename MODULEENTRY32A
#include "bcb6_std_vector_template.h"

#ifdef USING_NAMESPACE_BCB6_STD
#define vector_MODULEENTRY32A_push_back bcb6_std_vector_MODULEENTRY32A_push_back
#endif

EXTERN_C void __fastcall bcb6_std_vector_MODULEENTRY32A_push_back(bcb6_std_vector_MODULEENTRY32A *heapList, MODULEENTRY32A *heapListData);
