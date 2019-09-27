#ifndef MODULO
#  define MODULO        0
#  define LBL_EVN(name) evn_##name
#  define LBL_ODD(name) odd_##name
#  define LBL           LBL_EVN
#else
#  undef  MODULO
#  define MODULO        1
#  undef  LBL
#  define LBL           LBL_ODD
#endif

#if !MODULO
__declspec(naked) static int __cdecl wcsicmpSSE2(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

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
		jz      LBL_EVN(word_loop_entry)
		inc     eax
		dec     esi
		jmp     LBL_ODD(word_loop_entry)
#endif

		align   16
	LBL(word_loop):
		mov     dx, word ptr [eax + esi]
		mov     cx, word ptr [eax - MODULO]
		sub     edx, ecx
		jnz     LBL(word_compare_insensitive)
		test    ecx, ecx
		jz      LBL(return_equal)
	LBL(word_loop_increment):
		lea     ebx, [eax + esi + 2]
		add     eax, 2
	LBL(word_loop_entry):
		test    eax, 15                                  // use only eax for 'test reg, imm'
		jnz     LBL(word_loop)
		and     ebx, PAGE_SIZE - 1
		jmp     LBL(xmmword_loop)

		align   16
	LBL(word_compare_insensitive):
		cmp     edx, 'a' - 'A'
		je      LBL(word_compare_above)
		cmp     edx, 'A' - 'a'
		jne     LBL(word_not_equal)
		xor     edx, edx
		lea     ebx, [ecx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     LBL(word_loop_increment)
		sub     ecx, 'A'
		mov     eax, ebx
		jmp     LBL(secondary_to_lower)

		align   16
	LBL(word_compare_above):
		xor     edx, edx
		lea     ebx, [ecx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     LBL(word_loop_increment)
		mov     ecx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     LBL(primary_to_lower)

		align   16
	LBL(word_not_equal):
		lea     eax, [edx + ecx - 'A']
		sub     ecx, 'A'
		jmp     LBL(secondary_to_lower)

		align   16
	LBL(return_equal):
		xor     eax, eax
		pop     esi
		pop     ebx
		ret

		align   16
	LBL(xmmword_loop):
		cmp     ebx, PAGE_SIZE - 15
		ja      LBL(word_loop)                          // jump if cross pages
		movdqu  xmm3, xmmword ptr [eax + esi]           // enter 16 byte
#if !MODULO
		movdqa  xmm1, xmmword ptr [eax - MODULO]        //
#else
		movdqu  xmm1, xmmword ptr [eax - MODULO]        //
#endif
		movdqa  xmm0, xmm3                              // copy
		pcmpeqw xmm3, xmm6                              // compare 8 words with zero
		movdqa  xmm2, xmm0                              // copy
		pmovmskb ecx, xmm3                              // get one bit for each byte result
		movdqa  xmm3, xmm1                              // copy
		psubw   xmm0, xmm4                              // all words less than 'A'
		psubw   xmm1, xmm4                              //
		psubusw xmm0, xmm5                              // and 'Z' will be reset
		psubusw xmm1, xmm5                              //
		pcmpeqw xmm0, xmm6                              // xmm0 = (word >= 'A' && word <= 'Z') ? 0xFFFF : 0x0000
		pcmpeqw xmm1, xmm6                              //
		pand    xmm0, xmm7                              // assign a mask for the appropriate words
		pand    xmm1, xmm7                              //
		pxor    xmm0, xmm2                              // negation of the 5th bit - lowercase letters
		pxor    xmm1, xmm3                              //
		pcmpeqw xmm0, xmm1                              // compare
		pmovmskb edx, xmm0                              // get one bit for each byte result
		xor     edx, 0FFFFH
		jnz     LBL(xmmword_not_equal)
		test    ecx, ecx
		jnz     LBL(return_equal)
		lea     ebx, [eax + esi + 16]
		add     eax, 16
		and     ebx, PAGE_SIZE - 1
		jmp     LBL(xmmword_loop)

		align   16
	LBL(xmmword_not_equal):
		test    ecx, ecx
		jz      LBL(xmmword_has_not_null)
		bsf     ecx, ecx
		mov     ebx, 0FFFFH
		xor     ecx, 15
		shr     ebx, cl
		and     edx, ebx
		jz      LBL(return_equal)
		xor     ecx, ecx
	LBL(xmmword_has_not_null):
		bsf     edx, edx
		add     edx, eax
		xor     eax, eax
		mov     cx, word ptr [edx - MODULO]
		mov     ax, word ptr [edx + esi]
		sub     ecx, 'A'
		sub     eax, 'A'
	LBL(secondary_to_lower):
		cmp     ecx, 'Z' - 'A'
		ja      LBL(primary_to_lower)
		add     ecx, 'a' - 'A'
	LBL(primary_to_lower):
		cmp     eax, 'Z' - 'A'
		ja      LBL(return_difference)
		add     eax, 'a' - 'A'
	LBL(return_difference):
		sub     eax, ecx
		pop     esi
		pop     ebx
		ret

#if !MODULO
#  include __FILE__
#else
		#undef string1
		#undef string2
	}
}
#  undef  MODULO
#  undef  LBL_EVN
#  undef  LBL_ODD
#  undef  LBL
#endif
