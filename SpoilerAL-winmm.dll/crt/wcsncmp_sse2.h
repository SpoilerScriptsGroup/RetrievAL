#if !MODULO
#define MODULO            0
#define FUNCTION_NAME     wcsncmpSSE2
#define ODD_FUNCTION_NAME wcsncmpSSE2_odd_address
#define LBL_EVEN(name)    even_##name
#define LBL_ODD(name)     odd_##name
#define LBL               LBL_EVEN
static int __cdecl ODD_FUNCTION_NAME(const wchar_t *string1, const wchar_t *string2, size_t count);
#else
#undef FUNCTION_NAME
#define FUNCTION_NAME     ODD_FUNCTION_NAME
#undef LBL
#define LBL               LBL_ODD
#endif

#if !MODULO
__declspec(naked) static int __cdecl FUNCTION_NAME(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 12]           // esi = string1
		mov     edi, dword ptr [string2 + 12]           // edi = string2
		mov     ebx, dword ptr [count + 12]             // ebx = count
		mov     eax, edi
		lea     esi, [esi + ebx * 2]                    // esi = end of string1
		lea     edi, [edi + ebx * 2]                    // edi = end of string2
		xor     ebx, -1                                 // ebx = -count - 1
		pxor    xmm2, xmm2
		and     eax, 1
		jz      LBL_EVEN(word_loop_increment)
		jmp     LBL_ODD(word_loop_increment)
#endif

		align   16
	LBL(word_loop):
		mov     cx, word ptr [esi + ebx * 2]
		mov     dx, word ptr [edi + ebx * 2]
		cmp     cx, dx
		jne     LBL(return_not_equal)
		cmp     cx, 0
		je      LBL(return_equal)
	LBL(word_loop_increment):
		inc     ebx
		jz      LBL(return_equal)
		lea     eax, [edi + ebx * 2 + MODULO]
		lea     ecx, [esi + ebx * 2]
		test    eax, 15                                 // use only eax for 'test reg, imm'
		jnz     LBL(word_loop)
		and     ecx, PAGE_SIZE - 1
		jmp     LBL(xmmword_loop)

		align   16
	LBL(return_not_equal):
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		pop     ebx
		ret

		align   16
	LBL(return_equal):
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	LBL(xmmword_loop):
		cmp     ecx, PAGE_SIZE - 16
		ja      LBL(word_loop)                          // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + ebx * 2]
#if !MODULO
		movdqa  xmm1, xmmword ptr [edi + ebx * 2]
#else
		movdqu  xmm1, xmmword ptr [edi + ebx * 2]
#endif
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm2
		pmovmskb eax, xmm0
		pmovmskb ecx, xmm1
		xor     eax, 0FFFFH
		jnz     LBL(xmmword_not_equal)
		test    ecx, ecx
		jnz     LBL(return_equal)
		add     ebx, 8
		jc      LBL(return_equal)
		lea     ecx, [esi + ebx * 2]
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
		and     eax, edx
		jz      LBL(return_equal)
	LBL(xmmword_has_not_null):
		bsf     eax, eax
		shr     eax, 1
		add     ebx, eax
		jc      LBL(return_equal)
		xor     eax, eax
		xor     edx, edx
		mov     ax, word ptr [esi + ebx * 2]
		mov     dx, word ptr [edi + ebx * 2]
		sub     eax, edx
		pop     edi
		pop     esi
		pop     ebx
		ret

#if !MODULO
#undef MODULO
#define MODULO 1
#include __FILE__
#else
		#undef string1
		#undef string2
		#undef count
	}
}
#undef MODULO
#undef FUNCTION_NAME
#undef ODD_FUNCTION_NAME
#undef LBL_EVEN
#undef LBL_ODD
#undef LBL
#endif
