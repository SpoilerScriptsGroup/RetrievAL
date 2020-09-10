#ifndef _RAND_H_
#define _RAND_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <stdint.h>

#ifndef __BORLANDC__
#define __msreturn
#endif

#if defined(_MSC_VER) && _MSC_VER < 1200
#define __forceinline static __inline
#endif

#ifdef __cplusplus
extern "C" {
#endif

#pragma warning(push)
#pragma warning(disable:4273)

#define srandom srand
#define random (long)rand32

void __cdecl srand(unsigned int seed);
int __cdecl rand();
uint8_t __cdecl rand8();
uint16_t __cdecl rand16();
uint32_t __cdecl rand32();
uint64_t __msreturn __cdecl rand64();
uint32_t __cdecl internal_randf32();
uint64_t __msreturn __cdecl internal_randf64();
__forceinline float randf32() { uint32_t r = internal_randf32(); return *(float *)&r; }
__forceinline double randf64() { uint64_t r = internal_randf64(); return *(double *)&r; }

#pragma warning(pop)

#ifdef __cplusplus
}
#endif

#endif	// _RAND_H_
