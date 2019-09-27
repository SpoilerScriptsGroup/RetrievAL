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
__declspec(naked) static int __cdecl wcscmpSSE2(const wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]            // esi = string1
		mov     eax, dword ptr [string2 + 8]            // eax = string2
		sub     esi, eax
		pxor    xmm2, xmm2
		test    eax, 1
		jz      LBL_EVN(word_loop_entry)
		inc     eax
		dec     esi
		jmp     LBL_ODD(word_loop_entry)
#endif

		align   16
	LBL(word_loop):
		mov     cx, word ptr [eax + esi]
		mov     dx, word ptr [eax - MODULO]
		cmp     cx, dx
		jne     LBL(return_not_equal)
		test    cx, cx
		jz      LBL(return_equal)
		lea     edi, [eax + esi + 2]
		add     eax, 2
	LBL(word_loop_entry):
		test    eax, 15                                 // use only eax for 'test reg, imm'
		jnz     LBL(word_loop)
		and     edi, PAGE_SIZE - 1
		jmp     LBL(xmmword_loop)

		align   16
	LBL(return_not_equal):
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		ret

		align   16
	LBL(return_equal):
		xor     eax, eax
		pop     edi
		pop     esi
		ret

		align   16
	LBL(xmmword_loop):
		cmp     edi, PAGE_SIZE - 16
		ja      LBL(word_loop)                          // jump if cross pages
		movdqu  xmm0, xmmword ptr [eax + esi]
#if !MODULO
		movdqa  xmm1, xmmword ptr [eax - MODULO]
#else
		movdqu  xmm1, xmmword ptr [eax - MODULO]
#endif
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm2
		pmovmskb edx, xmm0
		pmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     LBL(xmmword_not_equal)
		test    ecx, ecx
		jnz     LBL(return_equal)
		lea     edi, [eax + esi + 16]
		add     eax, 16
		and     edi, PAGE_SIZE - 1
		jmp     LBL(xmmword_loop)

		align   16
	LBL(xmmword_not_equal):
		test    ecx, ecx
		jz      LBL(xmmword_has_not_null)
		bsf     ecx, ecx
		mov     edi, 0FFFFH
		xor     ecx, 15
		shr     edi, cl
		and     edx, edi
		jz      LBL(return_equal)
	LBL(xmmword_has_not_null):
		bsf     edx, edx
		add     edx, eax
		xor     eax, eax
		xor     ecx, ecx
		pop     edi
		mov     ax, word ptr [edx + esi]
		mov     cx, word ptr [edx - MODULO]
		sub     eax, ecx
		pop     esi
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
