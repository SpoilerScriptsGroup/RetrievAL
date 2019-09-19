#include <intrin.h>

#pragma intrinsic(__debugbreak)
#ifndef _WIN64
#pragma intrinsic(_addcarry_u32)
#define _add_uintptr(a, b, out) _addcarry_u32(0, a, b, out)
#else
#pragma intrinsic(_addcarry_u64)
#define _add_uintptr(a, b, out) _addcarry_u64(0, a, b, out)
#endif

#pragma warning(disable:4163)
#pragma function(wmemmove)
#pragma function(memmove)

wchar_t * __cdecl wmemmove(wchar_t *dest, const wchar_t *src, size_t count)
{
	if (_add_uintptr(count, count, &count))
		__debugbreak();
	return memmove(dest, src, count);
}
