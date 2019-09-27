#if !MODULO
#define MODULO            0
#define FUNCTION_NAME     wmemicmpSSE2
#define ODD_FUNCTION_NAME wmemicmpSSE2_odd_address
static int __cdecl ODD_FUNCTION_NAME(const wchar_t *buffer1, const wchar_t *buffer2, size_t count);
#else
#undef FUNCTION_NAME
#define FUNCTION_NAME     ODD_FUNCTION_NAME
#endif

__declspec(naked) static int __cdecl FUNCTION_NAME(const wchar_t *buffer1, const wchar_t *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

#if !MODULO
		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     esi, dword ptr [buffer1 + 16]           // esi = buffer1
		mov     edi, dword ptr [buffer2 + 16]           // edi = buffer2
		mov     ebp, dword ptr [count + 16]             // ebp = count
		mov     eax, edi
		lea     esi, [esi + ebp * 2]                    // esi = end of buffer1
		lea     edi, [edi + ebp * 2]                    // edi = end of buffer2
		xor     ebp, -1                                 // ebp = -count - 1
		xor     edx, edx
		movdqa  xmm4, xmmword ptr [ahigh]
		movdqa  xmm5, xmmword ptr [azrange]
		pxor    xmm6, xmm6                              // set to zero
		movdqa  xmm7, xmmword ptr [casebit]             // bit to change
		and     eax, 1
		jz      word_loop_increment
		jmp     ODD_FUNCTION_NAME
#else
		xor     eax, eax
		jmp     word_loop_increment
#endif

		align   16
	word_loop:
		mov     ax, word ptr [esi + ebp * 2]
		mov     dx, word ptr [edi + ebp * 2]
		sub     eax, edx
		jnz     word_compare_insensitive
	word_loop_increment:
		inc     ebp
		jz      return_equal
		lea     ebx, [edi + ebp * 2 + MODULO]
		lea     ecx, [esi + ebp * 2]
		and     ebx, 15
		jnz     word_loop
		and     ecx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	word_compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      word_compare_above
		cmp     eax, 'A' - 'a'
		jne     word_not_equal
		xor     eax, eax
		lea     ecx, [edx - 'a']
		cmp     ecx, 'z' - 'a'
		jbe     word_loop_increment
		sub     edx, 'A'
		mov     eax, ecx
		jmp     secondary_to_lower

		align   16
	word_compare_above:
		xor     eax, eax
		lea     ecx, [edx - 'A']
		cmp     ecx, 'Z' - 'A'
		jbe     word_loop_increment
		mov     edx, ecx
		lea     eax, [ecx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	word_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
		jmp     secondary_to_lower

		align   16
	xmmword_loop:
		cmp     ecx, PAGE_SIZE - 15
		ja      word_loop                                // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + ebp * 2]        // enter 16 byte
#if !MODULO
		movdqa  xmm1, xmmword ptr [edi + ebp * 2]        //
#else
		movdqu  xmm1, xmmword ptr [edi + ebp * 2]        //
#endif
		movdqa  xmm2, xmm0                               // copy
		movdqa  xmm3, xmm1                               //
		psubw   xmm0, xmm4                               // all words less than 'A'
		psubw   xmm1, xmm4                               //
		psubusw xmm0, xmm5                               // and 'Z' will be reset
		psubusw xmm1, xmm5                               //
		pcmpeqw xmm0, xmm6                               // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm6                               //
		pand    xmm0, xmm7                               // assign a mask for the appropriate words
		pand    xmm1, xmm7                               //
		pxor    xmm0, xmm2                               // negation of the 5th bit - lowercase letters
		pxor    xmm1, xmm3                               //
		pcmpeqw xmm0, xmm1                               // compare
		pmovmskb ebx, xmm0                               // get one bit for each byte result
		xor     ebx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebp, 8
		jc      return_equal
		lea     ecx, [esi + ebp * 2]
		and     ecx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_not_equal:
		bsf     ebx, ebx
		shr     ebx, 1
		add     ebp, ebx
		jc      return_equal
		mov     dx, word ptr [edi + ebp * 2]
		mov     ax, word ptr [esi + ebp * 2]
		sub     edx, 'A'
		sub     eax, 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A'
		ja      primary_to_lower
		add     edx, 'a' - 'A'
	primary_to_lower:
		cmp     eax, 'Z' - 'A'
		ja      return_difference
		add     eax, 'a' - 'A'
	return_difference:
		sub     eax, edx
	return_equal:
		pop     esi
		pop     edi
		pop     ebp
		pop     ebx
		ret

		#undef buffer1
		#undef buffer2
		#undef count
	}
}

#if !MODULO
#undef MODULO
#define MODULO 1
#include __FILE__
#else
#undef MODULO
#undef FUNCTION_NAME
#undef ODD_FUNCTION_NAME
#endif
