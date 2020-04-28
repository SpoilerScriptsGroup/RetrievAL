#include <stddef.h>
#ifndef _M_IX86

int __cdecl strncmp(const char *string1, const char *string2, size_t count)
{
	unsigned char c1, c2;

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
#include "PageSize.h"

static int __cdecl strncmpSSE2(const char *string1, const char *string2, size_t count);
static int __cdecl strncmp386(const char *string1, const char *string2, size_t count);
static int __cdecl strncmpCPUDispatch(const char *string1, const char *string2, size_t count);

static int(__cdecl * strncmpDispatch)(const char *string1, const char *string2, size_t count) = strncmpCPUDispatch;

__declspec(naked) int __cdecl strncmp(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		jmp     dword ptr [strncmpDispatch]
	}
}

__declspec(naked) static int __cdecl strncmpSSE2(const char *string1, const char *string2, size_t count)
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
		mov     esi, dword ptr [string1 + 16]               // esi = string1
		mov     edi, dword ptr [string2 + 16]               // edi = string2
		mov     ebp, dword ptr [count + 16]                 // ebp = count
		xor     eax, eax                                    // eax = 0
		add     esi, ebp                                    // esi = end of string1
		add     edi, ebp                                    // edi = end of string2
		xor     ebp, -1                                     // ebp = -count - 1
		pxor    xmm2, xmm2
		jmp     byte_loop_increment

		align   16
	byte_loop:
		movzx   eax, byte ptr [esi + ebp]
		movzx   edx, byte ptr [edi + ebp]
		sub     eax, edx
		jnz     epilogue
		test    edx, edx
		jz      epilogue
	byte_loop_increment:
		inc     ebp
		jz      epilogue
		lea     eax, [edi + ebp]
		lea     ecx, [esi + ebp]
		test    eax, 3                                      // use only eax for 'test reg, imm'
		jnz     byte_loop
		and     ecx, PAGE_SIZE - 1

		align   16
	dword_loop:
		and     eax, 15
		jz      xmmword_loop
	dword_check_cross_pages:
		cmp     ecx, PAGE_SIZE - 4
		ja      byte_loop                                   // jump if cross pages
		mov     eax, dword ptr [esi + ebp]
		mov     edx, dword ptr [edi + ebp]
		sub     eax, edx
		jnz     byte_loop                                   // not equal
		add     ebp, 4
		jc      epilogue
		lea     ebx, [edx - 01010101H]
		lea     ecx, [esi + ebp]
		xor     edx, -1
		and     ebx, 80808080H
		and     ecx, PAGE_SIZE - 1
		lea     eax, [edi + ebp]
		and     edx, ebx
		jz      dword_loop
		xor     eax, eax
	epilogue:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	xmmword_loop:
		cmp     ecx, PAGE_SIZE - 16
		ja      dword_check_cross_pages                     // jump if cross pages
		movdqu  xmm0, xmmword ptr [esi + ebp]
		movdqa  xmm1, xmmword ptr [edi + ebp]
		pcmpeqb xmm0, xmm1
		pcmpeqb xmm1, xmm2
		pmovmskb edx, xmm0
		pmovmskb ecx, xmm1
		xor     edx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     epilogue
		add     ebp, 16
		jc      epilogue
		lea     ecx, [esi + ebp]
		and     ecx, PAGE_SIZE - 1
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
		jz      epilogue
	xmmword_has_not_null:
		bsf     edx, edx
		add     ebp, edx
		jc      epilogue
		xor     edx, edx
		mov     al, byte ptr [esi + ebp]
		mov     dl, byte ptr [edi + ebp]
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		sub     eax, edx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

__declspec(naked) static int __cdecl strncmp386(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		mov     esi, dword ptr [string1 + 12]               // esi = string1
		mov     edi, dword ptr [string2 + 12]               // edi = string2
		mov     ebx, dword ptr [count + 12]                 // ebx = count
		add     esi, ebx                                    // esi = end of string1
		add     edi, ebx                                    // edi = end of string2
		xor     ebx, -1                                     // ebx = -count - 1
		jmp     byte_loop_increment

		align   16
	byte_loop:
		mov     cl, byte ptr [esi + ebx]
		mov     dl, byte ptr [edi + ebx]
		cmp     cl, dl
		jne     return_not_equal
		test    cl, cl
		jz      return_equal
	byte_loop_increment:
		inc     ebx
		jz      return_equal
		lea     eax, [edi + ebx]
		lea     ecx, [esi + ebx]
		and     eax, 3
		jnz     byte_loop
		shl     ecx, 32 - BSF_PAGE_SIZE

		align   16
	dword_loop:
		cmp     ecx, (PAGE_SIZE - 4) shl (32 - BSF_PAGE_SIZE)
		ja      byte_loop                                   // jump if cross pages
		mov     eax, dword ptr [esi + ebx]
		mov     edx, dword ptr [edi + ebx]
		cmp     eax, edx
		jne     byte_loop                                   // not equal
		add     ebx, 4
		jc      return_equal
		mov     ecx, esi
		sub     eax, 01010101H
		add     ecx, ebx
		xor     edx, -1
		shl     ecx, 32 - BSF_PAGE_SIZE
		and     eax, 80808080H
		and     edx, eax
		jz      dword_loop
	return_equal:
		xor     eax, eax
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	return_not_equal:
		sbb     eax, eax
		pop     edi
		or      eax, 1
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

__declspec(naked) static int __cdecl strncmpCPUDispatch(const char *string1, const char *string2, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strncmpDispatch], offset strncmp386
		jmp     strncmp386

	L1:
		mov     dword ptr [strncmpDispatch], offset strncmpSSE2
		jmp     strncmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
