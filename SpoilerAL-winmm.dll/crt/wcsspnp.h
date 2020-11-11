#ifdef UTF16MAP
#ifndef _M_IX86
wchar_t * __cdecl _wcsspnp(const wchar_t *string, const wchar_t *control)
{
	const wchar_t *p;
	wchar_t       c;

	for (p = control; c = *(p++); )
		_bittestandset(UTF16MAP, c);
	do
		c = *(string++);
	while (_bittest(map, c));
	string = c ? string - 1 : NULL;
	for (p = control; c = *(p++); )
		UTF16MAP[c >> 5] = 0;
	return (wchar_t *)string;
}
#else
__declspec(naked) wchar_t * __cdecl _wcsspnp(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     eax, dword ptr [string]                     // eax = string
		mov     edx, dword ptr [control]                    // edx = control
		xor     ecx, ecx
		jmp     listinit

		// init char bit map
		align   16
	initnext:
		bts     dword ptr [UTF16MAP], ecx
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
		bt      dword ptr [UTF16MAP], ecx
		jc      dstnext                                     // found char, continue

		// return code
		mov     edx, dword ptr [control]                    // edx = control
		add     ecx, -1
		sbb     ecx, ecx
		sub     eax, 2
		and     eax, ecx
		xor     ecx, ecx
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
static wchar_t * __cdecl _wcsspnp(const wchar_t *string, const wchar_t *control)
{
	const wchar_t *p1, *p2;
	wchar_t       c1, c2;

	for (p1 = string; c1 = *(p1++); )
		for (p2 = control; (c2 = *(p2++)) != c1; )
			if (!c2)
				return (wchar_t *)p1 - 1;
	return NULL;
}
#else
__declspec(naked) static wchar_t * __cdecl _wcsspnp(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		mov     ecx, dword ptr [string + 8]
		mov     esi, dword ptr [control + 8]
		xor     eax, eax

		align   16
	outer_loop:
		mov     ax, word ptr [ecx]
		add     ecx, 2
		test    ax, ax
		jz      epilog
		mov     edx, esi

		align   16
	inner_loop:
		mov     bx, word ptr [edx]
		add     edx, 2
		cmp     bx, ax
		je      outer_loop
		test    bx, bx
		jnz     inner_loop
		lea     eax, [ecx - 2]
	epilog:
		pop     esi                                         // restore esi
		pop     ebx                                         // restore ebx
		ret                                                 // __cdecl return

		#undef string
		#undef control
	}
}
#endif
#endif

