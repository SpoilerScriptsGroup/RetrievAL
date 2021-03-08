#ifndef _M_IX86
#ifdef UTF16MAP
#ifdef DISABLE_UCRT
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

#endif
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
#ifdef DISABLE_UCRT
size_t __cdecl wcsspn(const wchar_t *string, const wchar_t *control)
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

#endif
wchar_t * __cdecl _wcsspnp(const wchar_t *string, const wchar_t *control)
{
	const wchar_t *p1, *p2;
	wchar_t       c1, c2;

	for (p1 = string; c1 = *(p1++); )
		for (p2 = control; (c2 = *(p2++)) != c1; )
			if (!c2)
				return (wchar_t *)p1 - 1;
	return NULL;
}
#endif
#else
static unsigned __int64 __fastcall internal_wcsspnp(const wchar_t *string, const wchar_t *control);

#ifdef DISABLE_UCRT
__declspec(naked) size_t __cdecl wcsspn(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     ecx, dword ptr [string]                     // ecx = string
		mov     edx, dword ptr [control]                    // edx = control
		call    internal_wcsspnp

		// Return code
		mov     ecx, dword ptr [string]                     // ecx = string
		mov     eax, edx
		sub     eax, ecx
		shr     eax, 1
		ret

		#undef string
		#undef control
	}
}

#endif
__declspec(naked) wchar_t * __cdecl _wcsspnp(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  (esp + 4)
		#define control (esp + 8)

		mov     ecx, dword ptr [string]                     // ecx = string
		mov     edx, dword ptr [control]                    // edx = control
		call    internal_wcsspnp

		// Return code
		add     eax, -1
		sbb     eax, eax
		and     eax, edx
		ret

		#undef string
		#undef control
	}
}

#ifdef UTF16MAP
__declspec(naked) static unsigned __int64 __fastcall internal_wcsspnp(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  ecx
		#define control edx

		push    edx
		xor     eax, eax
		jmp     init

		// Set control char bits in map
		align   16
	initnext:
		bts     dword ptr [UTF16MAP], eax
	init:
		mov     ax, word ptr [edx]
		add     edx, 2
		test    eax, eax
		jnz     initnext

		mov     edx, ecx                                    // edx = string
		pop     ecx                                         // ecx = control

		// Loop through comparing source string with control bits
		align   16
	dstnext:
		mov     ax, word ptr [edx]
		add     edx, 2
		bt      dword ptr [UTF16MAP], eax
		jc      dstnext                                     // found char, continue

		push    eax
		jmp     clear

		// Clear control char bits in map
		align   16
	clearnext:
		shr     eax, 5
		mov     dword ptr [UTF16MAP + eax * 4], 0
		xor     eax, eax
	clear:
		mov     ax, word ptr [ecx]
		add     ecx, 2
		test    eax, eax
		jnz     clearnext
		pop     eax
		sub     edx, 2
		ret                                                 // __fastcall return

		#undef string
		#undef control
	}
}
#else
__declspec(naked) static unsigned __int64 __fastcall internal_wcsspnp(const wchar_t *string, const wchar_t *control)
{
	__asm
	{
		#define string  ecx
		#define control edx

		push    ebx                                         // preserve ebx
		push    esi                                         // preserve esi
		xor     eax, eax
		jmp     outer_loop

		align   16
	outer_loop:
		mov     ax, word ptr [ecx]
		add     ecx, 2
		test    eax, eax
		jz      epilog
		mov     esi, edx

		align   16
	inner_loop:
		mov     bx, word ptr [esi]
		add     esi, 2
		cmp     bx, ax
		je      outer_loop
		test    bx, bx
		jnz     inner_loop
	epilog:
		mov     edx, ecx
		pop     esi                                         // restore esi
		sub     edx, 2
		pop     ebx                                         // restore ebx
		ret                                                 // __fastcall return

		#undef string
		#undef control
	}
}
#endif
#endif
