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
size_t __cdecl wcsspn(const wchar_t *string, const wchar_t *control)
{
	static size_t __cdecl wcsspn_map(const wchar_t *string, const wchar_t *control);
	static size_t __cdecl wcsspn_generic(const wchar_t *string, const wchar_t *control);

	return (utf16map || (utf16map = (unsigned long *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 0x10000 / CHAR_BIT))) ?
		wcsspn_map(string, control) :
		wcsspn_generic(string, control);
}
#else
__declspec(naked) size_t __cdecl wcsspn(const wchar_t *string, const wchar_t *control)
{
	static size_t __cdecl wcsspn_map(const wchar_t *string, const wchar_t *control);
	static size_t __cdecl wcsspn_generic(const wchar_t *string, const wchar_t *control);

	__asm
	{
		cmp     dword ptr [utf16map], 0
		jne     wcsspn_map
		push    0x10000 / CHAR_BIT
		push    HEAP_ZERO_MEMORY
		push    dword ptr [hHeap]
		call    HeapAlloc
		test    eax, eax
		jz      wcsspn_generic
		mov     dword ptr [utf16map], eax
		jmp     wcsspn_map
	}
}
#endif

#ifndef _M_IX86
static size_t __cdecl wcsspn_map(const wchar_t *string, const wchar_t *control)
{
	const wchar_t *p;
	wchar_t       c;
	size_t        index;

	for (p = control; c = *(p++); )
		_bittestandset(utf16map, c);
	index = -1;
	string++;
	do
		c = string[index++];
	while (_bittest(utf16map, c));
	for (p = control; c = *(p++); )
		utf16map[c >> 5] = 0;
	return index;
}
#else
__declspec(naked) static size_t __cdecl wcsspn_map(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		push    ebx                                         // preserve ebx
		xor     ecx, ecx
		mov     ebx, dword ptr [utf16map]                   // ebx = utf16map
		mov     edx, dword ptr [string + 4]                 // edx = string
		mov     eax, dword ptr [control + 4]                // eax = control
		jmp     listinit

		// init char bit map
		align   16
	initnext:
		bts     dword ptr [ebx], ecx
	listinit:
		mov     cx, word ptr [eax]
		add     eax, 2
		test    cx, cx
		jnz     initnext

		or      eax, -1                                     // set eax to -1
		add     edx, 2                                      // edx = string + 1

		// Loop through comparing source string with control bits
		align   16
	dstnext:
		mov     cx, word ptr [edx + eax * 2]
		inc     eax
		bt      dword ptr [ebx], ecx
		jc      dstnext                                     // found char, continue

		mov     edx, dword ptr [control + 4]                // edx = control
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
static size_t __cdecl wcsspn_generic(const wchar_t *string, const wchar_t *control)
{
	size_t        n;
	const wchar_t *p1, *p2;
	wchar_t       c1, c2;

	n = -1;
	for (p1 = string + 1; c1 = p1[n++]; )
		for (p2 = control; (c2 = *(p2++)) != c1; )
			if (!c2)
				goto DONE;
DONE:
	return n;
}
#else
__declspec(naked) static size_t __cdecl wcsspn_generic(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		mov     eax, -1
		mov     edi, dword ptr [string + 12]
		mov     ebx, dword ptr [control + 12]
		add     edi, 2

		align   16
	outer_loop:
		mov     dx, word ptr [edi + eax * 2]
		inc     eax
		test    dx, dx
		jz      epilogue
		mov     esi, ebx

		align   16
	inner_loop:
		mov     cx, word ptr [esi]
		add     esi, 2
		cmp     cx, dx
		je      outer_loop
		test    cx, cx
		jnz     inner_loop
	epilogue:
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret                                                 // __cdecl return

		#undef string
		#undef control
	}
}
#endif
