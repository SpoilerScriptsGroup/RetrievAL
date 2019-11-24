#include <memory.h>
#include <tchar.h>

#ifdef _UNICODE
#include <intrin.h>
#define type wchar_t
#define _tmempcpy wmempcpy
#pragma intrinsic(__debugbreak)
#ifndef _WIN64
#pragma intrinsic(_addcarry_u32)
#define _add_uintptr(a, b, out) _addcarry_u32(0, a, b, out)
#else
#pragma intrinsic(_addcarry_u64)
#define _add_uintptr(a, b, out) _addcarry_u64(0, a, b, out)
#endif
#else
#define type void
#define _tmempcpy mempcpy
#endif

#pragma function(memcpy)

type * __cdecl _tmempcpy(type *dest, type *src, size_t count)
{
#ifdef _UNICODE
	if (_add_uintptr(count, count, &count))
		__debugbreak();
#endif
	return (type *)((char *)memcpy(dest, src, count) + count);
}
