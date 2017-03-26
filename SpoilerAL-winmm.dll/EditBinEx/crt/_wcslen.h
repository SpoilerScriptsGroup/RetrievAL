#ifndef _WCSLEN_H_
#define _WCSLEN_H_

#ifdef _M_IX86
__forceinline size_t __cdecl _wcslen(const wchar_t *string)
{
	__asm
	{
		xor     eax, eax
		mov     edi, DWORD PTR string
		mov     ecx, -1
		repne   scasw
		dec     eax
		inc     ecx
		xor     eax, ecx
	}
}
#else
#pragma intrinsic(wcslen)
#define _wcslen wcslen
#endif

#endif
