#ifndef _OPTIMIZEALLOCATOR_H_
#define _OPTIMIZEALLOCATOR_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define OPTIMIZE_ALLOCATOR 1

#include <windows.h>

#if OPTIMIZE_ALLOCATOR
EXTERN_C void __cdecl OptimizeAllocator();
#else
#define OptimizeAllocator()
#endif

#endif	// _OPTIMIZEALLOCATOR_H_
