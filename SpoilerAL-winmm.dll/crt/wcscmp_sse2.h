#if !MODULO
#define MODULO            0
#define FUNCTION_NAME     wcscmpSSE2
#define ODD_FUNCTION_NAME wcscmpSSE2_odd_address
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
		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]            // esi = string1
		mov     eax, dword ptr [string2 + 8]            // eax = string2
		sub     esi, eax
		pxor    xmm2, xmm2
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
		mov     cx, word ptr [eax + esi]
		mov     dx, word ptr [eax - MODULO]
		cmp     cx, dx
		jne     return_not_equal
		test    cx, cx
		jz      return_equal
		lea     edi, [eax + esi + 2]
		add     eax, 2
	word_loop_entry:
		test    eax, 15                                 // use only eax for 'test reg, imm'
		jnz     word_loop
		and     edi, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	return_not_equal:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		ret

		align   16
	return_equal:
		xor     eax, eax
		pop     edi
		pop     esi
		ret

		align   16
	xmmword_loop:
		cmp     edi, PAGE_SIZE - 16
		ja      word_loop                               // jump if cross pages
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
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     return_equal
		lea     edi, [eax + esi + 16]
		add     eax, 16
		and     edi, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     edi, 0FFFFH
		xor     ecx, 15
		shr     edi, cl
		and     edx, edi
		jz      return_equal
	xmmword_has_not_null:
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
