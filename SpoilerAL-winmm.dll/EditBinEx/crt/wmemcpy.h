#ifndef wmemcpy
#if _MSC_VER > 1200
#include <intrin.h>
#pragma intrinsic(__movsw)
#else
#if _MSC_VER >= 600
__forceinline
#else
static __inline
#endif
void __movsw(unsigned short *Dest, const unsigned short *Source, size_t Count)
{
	__asm
	{
		mov     ecx, DWORD PTR Count
		mov     esi, DWORD PTR Source
		mov     edi, DWORD PTR Dest
		rep     movsw
	}
}
#endif
#define wmemcpy __movsw
#endif
