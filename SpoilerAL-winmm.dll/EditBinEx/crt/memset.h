#ifndef memset
#if _MSC_VER > 1200
#include <intrin.h>
#pragma intrinsic(__stosb)
#else
#if _MSC_VER >= 600
__forceinline
#else
__inline
#endif
void __stosb(unsigned char* Dest, unsigned char Data, size_t Count)
{
	__asm
	{
		xor     eax, eax
		mov     ecx, DWORD PTR Count
		mov     edi, DWORD PTR Dest
		rep     stos
	}
}
#endif
#define memset __stosb
#endif
