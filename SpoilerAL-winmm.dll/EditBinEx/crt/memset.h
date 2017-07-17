#ifndef memset
#if _MSC_VER > 1200
#include <intrin.h>
#pragma intrinsic(__stosb)
#else
#if _MSC_VER >= 600
__forceinline
#else
static __inline
#endif
void __stosb(unsigned char* Dest, unsigned char Data, size_t Count)
{
	__asm
	{
		mov     ecx, dword ptr [Count]
		mov     al, byte ptr [Data]
		mov     edi, dword ptr [Dest]
		rep     stosb
	}
}
#endif
#define memset __stosb
#endif
