#include "noinline_wchar.h"
#include <memory.h>

#ifndef _M_IX86
wchar_t * __cdecl _wmemccpy(wchar_t *dest, const wchar_t *src, wchar_t c, size_t count)
{
	void *p;

	if (!count)
		return NULL;
	if (p = wmemchr(src, c, count))
		p = (char *)dest + (count = (char *)p - (char *)src + sizeof(wchar_t));
	else
		count *= sizeof(wchar_t);
	memmove(dest, src, count);
	return p;
}
#else
#pragma function(memmove)

__declspec(naked) wchar_t * __cdecl _wmemccpy(wchar_t *dest, const wchar_t *src, wchar_t c, size_t count)
{
	__asm
	{
		#define dest  (esp + 4)
		#define src   (esp + 8)
		#define c     (esp + 12)
		#define count (esp + 16)

		mov     eax, dword ptr [count]                  // load count
		mov     ecx, dword ptr [c]                      // load c
		test    eax, eax                                // compare count with 0
		jz      retnull                                 // jump if count == 0
		push    ebx                                     // store register
		push    esi                                     //
		mov     ebx, dword ptr [src + 8]                // load src
		mov     esi, eax                                // copy count
		push    eax                                     // push parameters
		push    ecx                                     //
		push    ebx                                     //
		call    wmemchr                                 // call wmemchr function
		mov     ecx, dword ptr [dest + 20]              // load dest
		mov     edx, ebx                                // copy src
		mov     ebx, eax                                // p = result of memchr
		mov     dword ptr [esp], ecx                    // store the dest of memmove
		test    eax, eax                                // compare the result of memchr with NULL
		jnz     found                                   // jump if the result of memchr != NULL
		add     esi, esi                                // count *= 2
		jmp     copy

	found:
		sub     ebx, edx                                // calculation
		add     ecx, 2                                  //
		lea     esi, [ebx + 2]                          // count = p - src + 2
		add     ebx, ecx                                // p = dest + count
	copy:
		mov     dword ptr [esp + 4], edx                // store the src of memmove
		mov     dword ptr [esp + 8], esi                // store the count of memmove
		call    memmove                                 // call memmove function
		add     esp, 12                                 // restore esp register
		mov     eax, ebx                                // set return value
		pop     esi                                     // restore register
		pop     ebx                                     //
	retnull:
		ret                                             // __cdecl return

		#undef dest
		#undef src
		#undef c
		#undef count
	}
}
#endif
