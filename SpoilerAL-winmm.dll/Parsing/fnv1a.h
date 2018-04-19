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

#define _FNV1A(type, ret, data, size, basis, prime)         \
do                                                          \
{                                                           \
    const uint8_t *__restrict _data = (const void *)(data); \
    size_t                    _size = size;                 \
    type                      _hash;                        \
                                                            \
    _hash = basis;                                          \
    while (_size--)                                         \
        _hash = (_hash ^ *(_data++)) * (prime);             \
    (ret) = _hash;                                          \
} while (0)

#define FNV1A32(ret, data, size) _FNV1A(uint32_t, ret, data, size, FNV1A32_BASIS, FNV1A32_PRIME)
#define FNV1A64(ret, data, size) _FNV1A(uint64_t, ret, data, size, FNV1A64_BASIS, FNV1A64_PRIME)

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _M_IX86
uint32_t __msfastcall fnv1a32(const void *data, size_t size);
uint64_t __msreturn __msfastcall fnv1a64(const void *data, size_t size);
#else
__forceinline uint32_t fnv1a32(const void *data, size_t size)
{
	uint32_t ret;
	FNV1A32(ret, data, size);
	return ret;
}
__forceinline uint64_t fnv1a64(const void *data, size_t size)
{
	uint64_t ret;
	FNV1A64(ret, data, size);
	return ret;
}
#endif

#ifdef __cplusplus
}
#endif

#ifdef _WIN64
#define FNV1A FNV1A64
#define fnv1a fnv1a64;
#else
#define FNV1A FNV1A32
#define fnv1a fnv1a32;
#endif

#endif	// _FNV1A_H_
