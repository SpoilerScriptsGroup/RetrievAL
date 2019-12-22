#ifndef _RAND_H_
#define _RAND_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void __cdecl srand(unsigned int seed);
int __cdecl rand();
uint8_t __cdecl rand8();
uint16_t __cdecl rand16();
uint32_t __cdecl rand32();
uint64_t __cdecl rand64();
float __cdecl randf32();
double __cdecl randf64();

#ifdef __cplusplus
}
#endif

#endif	// _RAND_H_
