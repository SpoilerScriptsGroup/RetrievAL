#include <wchar.h>
#ifndef _M_IX86

int __cdecl wcsncmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	unsigned short c1, c2;

	string1 += count;
	string2 += count;
	count ^= -1;
	do
		if (!++count)
			break;
		else if ((c1 = string1[count]) != (c2 = string2[count]))
			return (int)c1 - (int)c2;
	while (c1);
	return 0;
}
#else
#include "page.h"

static int __cdecl wcsncmpSSE2(const wchar_t *string1, const wchar_t *string2, size_t count);
static int __cdecl wcsncmp386(const wchar_t *string1, const wchar_t *string2, size_t count);
static int __cdecl wcsncmpCPUDispatch(const wchar_t *string1, const wchar_t *string2, size_t count);

static int(__cdecl * wcsncmpDispatch)(const wchar_t *string1, const wchar_t *string2, size_t count) = wcsncmpCPUDispatch;

__declspec(naked) int __cdecl wcsncmp(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		jmp     dword ptr [wcsncmpDispatch]
	}
}

__declspec(naked) static int __cdecl wcsncmpSSE2(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax                                    // eax = 0
		mov     ebx, dword ptr [count + 12]                 // ebx = count
		mov     edi, dword ptr [string2 + 12]               // edi = string2
		mov     esi, dword ptr [string1 + 12]               // esi = string1
		lea     edi, [edi + ebx * 2]                        // edi = end of string2
		lea     esi, [esi + ebx * 2]                        // esi = end of string1
		xor     ebx, -1                                     // ebx = -count - 1
		pxor    xmm2, xmm2
		jmp     word_loop_increment

		align   16
	word_loop:
		movzx   eax, word ptr [esi + ebx * 2]
		movzx   edx, word ptr [edi + ebx * 2]
		sub     eax, edx
		jnz     epilogue
		test    edx, edx
		jz      epilogue
	word_loop_increment:
		inc     ebx
		jz      epilogue
		lea     edx, [edi + ebx * 2 + 1]
		lea     ecx, [esi + ebx * 2]
		and     edx, 14
		jnz     word_loop
		shl     ecx, 32 - PAGE_SHIFT
		mov     edx, edi
		and     edx, 1
		jnz     unaligned_xmmword_loop

		align   16
	aligned_xmmword_loop:
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + ebx * 2]
		movdqa  xmm1, xmmword ptr [edi + ebx * 2]
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm2
		pmovmskb edx, xmm0
		pmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilogue
		add     ebx, 8
		jc      epilogue
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT
		jmp     aligned_xmmword_loop

		align   16
	unaligned_xmmword_loop:
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     word_loop                                   // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + ebx * 2]
		movdqu  xmm1, xmmword ptr [edi + ebx * 2]
		pcmpeqw xmm0, xmm1
		pcmpeqw xmm1, xmm2
		pmovmskb edx, xmm0
		pmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilogue
		add     ebx, 8
		jc      epilogue
		lea     ecx, [esi + ebx * 2]
		shl     ecx, 32 - PAGE_SHIFT
		jmp     unaligned_xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     eax, 0FFFFH
		xor     ecx, 15
		shr     eax, cl
		and     eax, edx
		jz      epilogue
		mov     edx, eax
		xor     eax, eax
	xmmword_has_not_null:
		bsf     edx, edx
		shr     edx, 1
		add     ebx, edx
		jc      epilogue
		movzx   eax, word ptr [esi + ebx * 2]
		movzx   edx, word ptr [edi + ebx * 2]
		sub     eax, edx
	epilogue:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

__declspec(naked) static int __cdecl wcsncmp386(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 8]                // esi = string1
		mov     edi, dword ptr [string2 + 8]                // edi = string2
		mov     ecx, dword ptr [count + 8]                  // ecx = count
		xor     eax, eax                                    // eax = 0
		lea     esi, [esi + ecx * 2]                        // esi = end of string1
		lea     edi, [edi + ecx * 2]                        // edi = end of string2
		xor     ecx, -1                                     // ecx = -count - 1
		xor     edx, edx                                    // edx = 0

		align   16
	loop_begin:
		inc     ecx
		jz      epilogue
		mov     ax, word ptr [esi + ecx * 2]
		mov     dx, word ptr [edi + ecx * 2]
		sub     eax, edx
		jnz     epilogue
		test    edx, edx
		jnz     loop_begin
	epilogue:
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

__declspec(naked) static int __cdecl wcsncmpCPUDispatch(const wchar_t *string1, const wchar_t *string2, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [wcsncmpDispatch], offset wcsncmp386
		jmp     wcsncmp386

	L1:
		mov     dword ptr [wcsncmpDispatch], offset wcsncmpSSE2
		jmp     wcsncmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
