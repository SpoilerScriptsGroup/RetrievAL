#if _MSC_VER > 1200
#include <string.h>
#pragma intrinsic(wcslen)
#elif !defined(wcslen)
#define wcslen inline_wcslen
#if _MSC_VER >= 600
__forceinline
#else
static __inline
#endif
size_t inline_wcslen(const wchar_t *string)
{
	__asm
	{
		xor     eax, eax
		mov     edi, dword ptr [string]
		mov     ecx, -1
		repne scasw
		dec     eax
		inc     ecx
		xor     eax, ecx
	}
}
#endif
