#if !MODULO
#define MODULO            0
#define FUNCTION_NAME     wcsicmpSSE2
#define ODD_FUNCTION_NAME wcsicmpSSE2_odd_address
static int __cdecl ODD_FUNCTION_NAME(const wchar_t *string1, const wchar_t *string2);
#else
#undef FUNCTION_NAME
#define FUNCTION_NAME     ODD_FUNCTION_NAME
#endif

__declspec(naked) static int __cdecl FUNCTION_NAME(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

#if !MODULO
		push    ebx
		push    esi
		mov     esi, dword ptr [string1 + 8]            // esi = string1
		mov     eax, dword ptr [string2 + 8]            // eax = string2
		mov     ebx, esi
		sub     esi, eax
		xor     edx, edx
		xor     ecx, ecx
		movdqa  xmm4, xmmword ptr [ahigh]
		movdqa  xmm5, xmmword ptr [azrange]
		pxor    xmm6, xmm6                              // set to zero
		movdqa  xmm7, xmmword ptr [casebit]             // bit to change
		test    eax, 1
		jz      word_loop_entry
		jmp     ODD_FUNCTION_NAME
#else
		inc     eax
		dec     esi
		jmp     word_loop_entry
#endif

		align   16
	word_loop:
		mov     dx, word ptr [eax + esi]
		mov     cx, word ptr [eax - MODULO]
		sub     edx, ecx
		jnz     word_compare_insensitive
		test    ecx, ecx
		jz      return_equal
	word_loop_increment:
		lea     ebx, [eax + esi + 2]
		add     eax, 2
	word_loop_entry:
		test    eax, 15                                  // use only eax for 'test reg, imm'
		jnz     word_loop
		and     ebx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	word_compare_insensitive:
		cmp     edx, 'a' - 'A'
		je      word_compare_above
		cmp     edx, 'A' - 'a'
		jne     word_not_equal
		xor     edx, edx
		lea     ebx, [ecx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     word_loop_increment
		sub     ecx, 'A'
		mov     eax, ebx
		jmp     secondary_to_lower

		align   16
	word_compare_above:
		xor     edx, edx
		lea     ebx, [ecx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     word_loop_increment
		mov     ecx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	word_not_equal:
		lea     eax, [edx + ecx - 'A']
		sub     ecx, 'A'
		jmp     secondary_to_lower

		align   16
	return_equal:
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	xmmword_loop:
		cmp     ebx, PAGE_SIZE - 15
		ja      word_loop                                // jump if cross pages
		movdqu  xmm3, xmmword ptr [eax + esi]            // enter 16 byte
#if !MODULO
		movdqa  xmm1, xmmword ptr [eax - MODULO]         //
#else
		movdqu  xmm1, xmmword ptr [eax - MODULO]         //
#endif
		movdqa  xmm0, xmm3                               // copy
		pcmpeqw xmm3, xmm6                               // compare 8 words with zero
		movdqa  xmm2, xmm0                               // copy
		pmovmskb ecx, xmm3                               // get one bit for each byte result
		movdqa  xmm3, xmm1                               // copy
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
		pmovmskb edx, xmm0                               // get one bit for each byte result
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     return_equal
		lea     ebx, [eax + esi + 16]
		add     eax, 16
		and     ebx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     ebx, 0FFFFH
		xor     ecx, 15
		shr     ebx, cl
		and     edx, ebx
		jz      return_equal
		xor     ecx, ecx
	xmmword_has_not_null:
		bsf     edx, edx
		add     edx, eax
		xor     eax, eax
		mov     cx, word ptr [edx - MODULO]
		mov     ax, word ptr [edx + esi]
		sub     ecx, 'A'
		sub     eax, 'A'
	secondary_to_lower:
		cmp     ecx, 'Z' - 'A'
		ja      primary_to_lower
		add     ecx, 'a' - 'A'
	primary_to_lower:
		cmp     eax, 'Z' - 'A'
		ja      return_difference
		add     eax, 'a' - 'A'
	return_difference:
		sub     eax, ecx
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
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
