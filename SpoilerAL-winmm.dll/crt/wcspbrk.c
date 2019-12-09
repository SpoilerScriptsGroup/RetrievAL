#include <windows.h>
#include <wchar.h>
#include <limits.h>

#ifndef _M_IX86
#include <intrin.h>
#pragma intrinsic(_bittest)
#pragma intrinsic(_bittestandset)
#else
#pragma warning(disable:4414)
#endif

#pragma warning(disable:4273)

extern HANDLE hHeap;
extern unsigned long *utf16map;	// 8 kilo byte

#ifndef _M_IX86
wchar_t * __cdecl wcspbrk(const wchar_t *string, const wchar_t *control)
{
	static wchar_t * __cdecl wcspbrk_map(const wchar_t *string, const wchar_t *control);
	static wchar_t * __cdecl wcspbrk_generic(const wchar_t *string, const wchar_t *control);

	return (utf16map || (utf16map = (unsigned long *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 0x10000 / CHAR_BIT))) ?
		wcspbrk_map(string, control) :
		wcspbrk_generic(string, control);
}
#else
__declspec(naked) wchar_t * __cdecl wcspbrk(const wchar_t *string, const wchar_t *control)
{
	static wchar_t * __cdecl wcspbrk_map(const wchar_t *string, const wchar_t *control);
	static wchar_t * __cdecl wcspbrk_generic(const wchar_t *string, const wchar_t *control);

	__asm
	{
		cmp     dword ptr [utf16map], 0
		jne     wcspbrk_map
		push    0x10000 / CHAR_BIT
		push    HEAP_ZERO_MEMORY
		push    dword ptr [hHeap]
		call    HeapAlloc
		test    eax, eax
		jz      wcspbrk_generic
		mov     dword ptr [utf16map], eax
		jmp     wcspbrk_map
	}
}
#endif

#ifndef _M_IX86
static wchar_t * __cdecl wcspbrk_map(const wchar_t *string, const wchar_t *control)
{
	const wchar_t *p;
	wchar_t       c;

	for (utf16map[0] = 1, p = control; c = *(p++); )
		_bittestandset(utf16map, c);
	do
		c = *(string++);
	while (!_bittest(utf16map, c));
	for (utf16map[0] = 0, p = control; c = *(p++); )
		utf16map[c >> 5] = 0;
	return *(--string) ? (wchar_t *)string : NULL;
}
#else
__declspec(naked) static wchar_t * __cdecl wcspbrk_map(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		push    ebx                                         // preserve ebx
		xor     ecx, ecx
		mov     ebx, dword ptr [utf16map]                   // ebx = utf16map
		mov     eax, dword ptr [string + 4]                 // eax = string
		mov     edx, dword ptr [control + 4]                // edx = control
		mov     dword ptr [ebx], 1
		jmp     listinit

		// init char bit map
		align   16
	initnext:
		bts     dword ptr [ebx], ecx
	listinit:
		mov     cx, word ptr [edx]
		add     edx, 2
		test    cx, cx
		jnz     initnext

		// Loop through comparing source string with control bits
		align   16
	dstnext:
		mov     cx, word ptr [eax]
		add     eax, 2
		bt      dword ptr [ebx], ecx
		jnc     dstnext                                     // did not find char, continue

		// return code
		xor     edx, edx
		cmp     ecx, 1
		adc     edx, -1
		sub     eax, 2
		and     eax, edx
		mov     edx, dword ptr [control + 4]                // edx = control
		mov     dword ptr [ebx], 0
		jmp     listclear

		// clear char bit map
		align   16
	clearnext:
		shr     ecx, 5
		mov     dword ptr [ebx + ecx * 4], 0
		xor     ecx, ecx
	listclear:
		mov     cx, word ptr [edx]
		add     edx, 2
		test    cx, cx
		jnz     clearnext
		pop     ebx                                         // restore ebx
		ret                                                 // __cdecl return

		#undef string
		#undef control
	}
}
#endif

#ifndef _M_IX86
static wchar_t * __cdecl wcspbrk_generic(const wchar_t *string, const wchar_t *control)
{
	const wchar_t *p1, *p2;
	wchar_t       c1, c2;

	for (p1 = string; c1 = *(p1++); )
		for (p2 = control; c2 = *(p2++); )
			if (c2 == c1)
				return (wchar_t *)p1 - 1;
	return NULL;
}
#else
__declspec(naked) static wchar_t * __cdecl wcspbrk_generic(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		mov     edx, dword ptr [string + 8]
		mov     ebx, dword ptr [control + 8]
		xor     eax, eax

		align   16
	outer_loop:
		mov     ax, word ptr [edx]
		add     edx, 2
		test    ax, ax
		jz      epilogue
		mov     esi, ebx

		align   16
	inner_loop:
		mov     cx, word ptr [esi]
		add     esi, 2
		test    cx, cx
		jz      outer_loop
		cmp     cx, ax
		jne     inner_loop
		lea     eax, [edx - 2]
	epilogue:
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret                                                 // __cdecl return

		#undef string
		#undef control
	}
}
#endif
