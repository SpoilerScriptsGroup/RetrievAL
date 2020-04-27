#include <memory.h>

#ifndef _M_IX86
void * __cdecl _memccpy(void *dest, const void *src, int c, size_t count)
{
	void *p;

	if (!count)
		return NULL;
	if (p = memchr(src, c, count))
		p = (char *)dest + (count = (char *)p - (char *)src + sizeof(char));
	memmove(dest, src, count);
	return p;
}
#else
#pragma function(memchr, memmove)

__declspec(naked) void * __cdecl _memccpy(void *dest, const void *src, int c, size_t count)
{
	__asm
	{
		#define dest  (esp + 4)
		#define src   (esp + 8)
		#define c     (esp + 12)
		#define count (esp + 16)

		mov     eax, dword ptr [count]                      // load count
		mov     ecx, dword ptr [c]                          // load c
		test    eax, eax                                    // compare count with 0
		jz      retnull                                     // jump if count == 0
		push    ebx                                         // store register
		push    esi                                         //
		mov     ebx, dword ptr [src + 8]                    // load src
		mov     esi, eax                                    // copy count
		push    eax                                         // push parameters
		push    ecx                                         //
		push    ebx                                         //
		call    memchr                                      // call memchr function
		mov     ecx, dword ptr [dest + 20]                  // load dest
		mov     edx, ebx                                    // copy src
		mov     ebx, eax                                    // p = result of memchr
		mov     dword ptr [esp], ecx                        // store the dest of memmove
		test    eax, eax                                    // compare the result of memchr with NULL
		jz      copy                                        // jump if the result of memchr == NULL
		sub     ebx, edx                                    // calculation
		inc     ecx                                         //
		lea     esi, [ebx + 1]                              // count = p - src + 1
		add     ebx, ecx                                    // p = dest + count
	copy:
		mov     dword ptr [esp + 4], edx                    // store the src of memmove
		mov     dword ptr [esp + 8], esi                    // store the count of memmove
		call    memmove                                     // call memmove function
		add     esp, 12                                     // restore esp register
		mov     eax, ebx                                    // set return value
		pop     esi                                         // restore register
		pop     ebx                                         //
	retnull:
		ret                                                 // __cdecl return

		#undef dest
		#undef src
		#undef c
		#undef count
	}
}
#endif
