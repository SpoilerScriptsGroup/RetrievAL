#ifndef _FNV1A_H_
#define _FNV1A_H_

#if _MSC_VER > 1000
#pragma once
#endif

#if !defined(__restrict) && ((defined(_MSC_VER) && _MSC_VER < 1400) || defined(__BORLANDC__))
#define __restrict
#endif

#ifndef __BORLANDC__
#ifndef __msfastcall
#define __msfastcall __fastcall
#endif
#ifndef __msreturn
#define __msreturn
#endif
#endif

#include <stddef.h>
#include <stdint.h>

#define FNV1A32_BASIS UINT32_C(0x811C9DC5)
#define FNV1A32_PRIME UINT32_C(0x01000193)
#define FNV1A64_BASIS UINT64_C(0xCBF29CE484222325)
#define FNV1A64_PRIME UINT64_C(0x00000100000001B3)

#define FNV1A_MACRO(type, ret, data, size, hash, prime)         \
do                                                              \
{                                                               \
    const uint8_t *__restrict __data  = (const void *)(data);   \
    size_t                    __size  = size;                   \
    type                      __hash  = hash;                   \
    type                      __prime = prime;                  \
                                                                \
    while (__size--)                                            \
        __hash = (__hash ^ *(__data++)) * __prime;              \
    (ret) = __hash;                                             \
} while (0)

#define FNV1A32(ret, data, size) FNV1A_MACRO(uint32_t, ret, data, size, FNV1A32_BASIS, FNV1A32_PRIME)
#define FNV1A64(ret, data, size) FNV1A_MACRO(uint64_t, ret, data, size, FNV1A64_BASIS, FNV1A64_PRIME)
#define FNV1A32COMBINE(ret, data, size, hash) FNV1A_MACRO(uint32_t, ret, data, size, hash, FNV1A32_PRIME)
#define FNV1A64COMBINE(ret, data, size, hash) FNV1A_MACRO(uint64_t, ret, data, size, hash, FNV1A64_PRIME)

__forceinline uint32_t inline_fnv1a32(const void *data, size_t size)
{
	uint32_t ret;
	FNV1A32(ret, data, size);
	return ret;
}
__forceinline uint64_t inline_fnv1a64(const void *data, size_t size)
{
	uint64_t ret;
	FNV1A64(ret, data, size);
	return ret;
}
__forceinline uint32_t inline_fnv1a32combine(const void *data, size_t size, uint32_t hash)
{
	uint32_t ret;
	FNV1A32COMBINE(ret, data, size, hash);
	return ret;
}
__forceinline uint64_t inline_fnv1a64combine(const void *data, size_t size, uint64_t hash)
{
	uint64_t ret;
	FNV1A64COMBINE(ret, data, size, hash);
	return ret;
}

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _M_IX86
uint32_t __msfastcall fnv1a32(const void *data, size_t size);
uint64_t __msreturn __msfastcall fnv1a64(const void *data, size_t size);
uint32_t __msfastcall fnv1a32combine(const void *data, size_t size, uint32_t hash);
uint64_t __msreturn __msfastcall fnv1a64combine(const void *data, size_t size, uint64_t hash);
#else
#define fnv1a32(data, size) inline_fnv1a32(data, size)
#define fnv1a64(data, size) inline_fnv1a64(data, size)
#define fnv1a32combine(data, size, hash) inline_fnv1a32combine(data, size, hash)
#define fnv1a64combine(data, size, hash) inline_fnv1a64combine(data, size, hash)
#endif

#ifdef __cplusplus
}
#endif

#ifdef _WIN64
#define FNV1A        FNV1A64
#define FNV1ACOMBINE FNV1A64COMBINE
#define fnv1a        fnv1a64
#define fnv1acombine fnv1a64combine
#else
#define FNV1A        FNV1A32
#define FNV1ACOMBINE FNV1A32COMBINE
#define fnv1a        fnv1a32
#define fnv1acombine fnv1a32combine
#endif

#endif	// _FNV1A_H_
