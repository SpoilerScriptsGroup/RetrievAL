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
__declspec(naked) static int __cdecl wcsnicmpSSE2(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 16]           // esi = string1
		mov     edi, dword ptr [string2 + 16]           // edi = string2
		mov     ebp, dword ptr [count + 16]             // ebp = count
		mov     eax, edi
		lea     esi, [esi + ebp * 2]                    // esi = end of string1
		lea     edi, [edi + ebp * 2]                    // edi = end of string2
		xor     ebp, -1                                 // ebp = -count - 1
		xor     edx, edx
		movdqa  xmm4, xmmword ptr [ahigh]
		movdqa  xmm5, xmmword ptr [azrange]
		pxor    xmm6, xmm6                              // set to zero
		movdqa  xmm7, xmmword ptr [casebit]             // bit to change
		and     eax, 1
		jz      LBL_EVN(word_loop_increment)
		xor     eax, eax
		jmp     LBL_ODD(word_loop_increment)
#endif

		align   16
	LBL(word_loop):
		mov     ax, word ptr [esi + ebp * 2]
		mov     dx, word ptr [edi + ebp * 2]
		sub     eax, edx
		jnz     LBL(word_compare_insensitive)
		test    edx, edx
		jz      LBL(return_equal)
	LBL(word_loop_increment):
		inc     ebp
		jz      LBL(return_equal)
		lea     ebx, [edi + ebp * 2 + MODULO]
		lea     ecx, [esi + ebp * 2]
		and     ebx, 15
		jnz     LBL(word_loop)
		and     ecx, PAGE_SIZE - 1
		jmp     LBL(xmmword_loop)

		align   16
	LBL(word_compare_insensitive):
		cmp     eax, 'a' - 'A'
		je      LBL(word_compare_above)
		cmp     eax, 'A' - 'a'
		jne     LBL(word_not_equal)
		xor     eax, eax
		lea     ecx, [edx - 'a']
		cmp     ecx, 'z' - 'a'
		jbe     LBL(word_loop_increment)
		sub     edx, 'A'
		mov     eax, ecx
		jmp     LBL(secondary_to_lower)

		align   16
	LBL(word_compare_above):
		xor     eax, eax
		lea     ecx, [edx - 'A']
		cmp     ecx, 'Z' - 'A'
		jbe     LBL(word_loop_increment)
		mov     edx, ecx
		lea     eax, [ecx + 'a' - 'A']
		jmp     LBL(primary_to_lower)

		align   16
	LBL(word_not_equal):
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
		jmp     LBL(secondary_to_lower)

		align   16
	LBL(xmmword_loop):
		cmp     ecx, PAGE_SIZE - 15
		ja      LBL(word_loop)                          // jump if cross pages
		movdqu  xmm3, xmmword ptr [esi + ebp * 2]       // enter 16 byte
#if !MODULO
		movdqa  xmm1, xmmword ptr [edi + ebp * 2]       //
#else
		movdqu  xmm1, xmmword ptr [edi + ebp * 2]       //
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
		pmovmskb ebx, xmm0                              // get one bit for each byte result
		xor     ebx, 0FFFFH
		jnz     LBL(xmmword_not_equal)
		test    ecx, ecx
		jnz     LBL(return_equal)
		add     ebp, 8
		jc      LBL(return_equal)
		lea     ecx, [esi + ebp * 2]
		and     ecx, PAGE_SIZE - 1
		jmp     LBL(xmmword_loop)

		align   16
	LBL(xmmword_not_equal):
		test    ecx, ecx
		jz      LBL(xmmword_has_not_null)
		bsf     ecx, ecx
		mov     edx, 0FFFFH
		xor     ecx, 15
		shr     edx, cl
		and     ebx, edx
		jz      LBL(return_equal)
		xor     edx, edx
	LBL(xmmword_has_not_null):
		bsf     ebx, ebx
		shr     ebx, 1
		add     ebp, ebx
		jc      LBL(return_equal)
		mov     dx, word ptr [edi + ebp * 2]
		mov     ax, word ptr [esi + ebp * 2]
		sub     edx, 'A'
		sub     eax, 'A'
	LBL(secondary_to_lower):
		cmp     edx, 'Z' - 'A'
		ja      LBL(primary_to_lower)
		add     edx, 'a' - 'A'
	LBL(primary_to_lower):
		cmp     eax, 'Z' - 'A'
		ja      LBL(return_difference)
		add     eax, 'a' - 'A'
	LBL(return_difference):
		sub     eax, edx
	LBL(return_equal):
		pop     esi
		pop     edi
		pop     ebp
		pop     ebx
		ret

#if !MODULO
#  include __FILE__
#else
		#undef string1
		#undef string2
		#undef count
	}
}
#  undef  MODULO
#  undef  LBL_EVN
#  undef  LBL_ODD
#  undef  LBL
#endif
