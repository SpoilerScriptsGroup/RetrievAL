#ifdef UTF16MAP
#ifndef _M_IX86
size_t __cdecl wcsspn(const wchar_t *string, const wchar_t *control)
{
	const wchar_t *p;
	wchar_t       c;
	size_t        index;

	for (p = control; c = *(p++); )
		_bittestandset(UTF16MAP, c);
	index = -1;
	string++;
	do
		c = string[index++];
	while (_bittest(UTF16MAP, c));
	for (p = control; c = *(p++); )
		UTF16MAP[c >> 5] = 0;
	return index;
}
#else
__declspec(naked) size_t __cdecl wcsspn(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     edx, dword ptr [string]                     // edx = string
		mov     eax, dword ptr [control]                    // eax = control
		xor     ecx, ecx
		jmp     listinit

		// init char bit map
		align   16
	initnext:
		bts     dword ptr [UTF16MAP], ecx
	listinit:
		mov     cx, word ptr [eax]
		add     eax, 2
		test    cx, cx
		jnz     initnext

		or      eax, -1                                     // eax = -1
		add     edx, 2                                      // edx = string + 1

		// Loop through comparing source string with control bits
		align   16
	dstnext:
		mov     cx, word ptr [edx + eax * 2]
		inc     eax
		bt      dword ptr [UTF16MAP], ecx
		jc      dstnext                                     // found char, continue

		mov     edx, dword ptr [control]                    // edx = control
		jmp     listclear

		// clear char bit map
		align   16
	clearnext:
		shr     ecx, 5
		mov     dword ptr [UTF16MAP + ecx * 4], 0
		xor     ecx, ecx
	listclear:
		mov     cx, word ptr [edx]
		add     edx, 2
		test    cx, cx
		jnz     clearnext
		ret                                                 // __cdecl return

		#undef string
		#undef control
	}
}
#endif
#else
#ifndef _M_IX86
static size_t __cdecl wcsspn(const wchar_t *string, const wchar_t *control)
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
__declspec(naked) static size_t __cdecl wcsspn(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		push    edi                                         // preserve edi
		mov     eax, -1
		mov     esi, dword ptr [string + 12]
		mov     edi, dword ptr [control + 12]
		add     esi, 2

		align   16
	outer_loop:
		mov     bx, word ptr [esi + eax * 2]
		inc     eax
		test    bx, bx
		jz      epilog
		mov     edx, edi

		align   16
	inner_loop:
		mov     cx, word ptr [edx]
		add     edx, 2
		cmp     cx, bx
		je      outer_loop
		test    cx, cx
		jnz     inner_loop
	epilog:
		pop     edi                                         // restore edi
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret                                                 // __cdecl return

		#undef string
		#undef control
	}
}
#endif
#endif

