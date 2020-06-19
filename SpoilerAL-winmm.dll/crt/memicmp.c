#include <stddef.h>
#ifndef _M_IX86
#include <ctype.h>

int __cdecl _memicmp(const void *buffer1, const void *buffer2, size_t count)
{
	int ret;

	(char *)buffer1 += count;
	(char *)buffer2 += count;
	count ^= -1;
	while (++count)
		if (ret = tolower(((unsigned char *)buffer1)[count]) - tolower(((unsigned char *)buffer2)[count]))
			return ret;
	return 0;
}
#else
#include "page.h"

static int __cdecl memicmpSSE2(const void *buffer1, const void *buffer2, size_t count);
static int __cdecl memicmp386(const void *buffer1, const void *buffer2, size_t count);
static int __cdecl memicmpCPUDispatch(const void *buffer1, const void *buffer2, size_t count);

static int(__cdecl * memicmpDispatch)(const void *buffer1, const void *buffer2, size_t count) = memicmpCPUDispatch;

extern const char xmmconst_upperA[16];
extern const char xmmconst_azrangeA[16];
extern const char xmmconst_casebitA[16];
#define upper   xmmconst_upperA
#define azrange xmmconst_azrangeA
#define casebit xmmconst_casebitA

__declspec(naked) int __cdecl _memicmp(const void *buffer1, const void *buffer2, size_t count)
{
	__asm
	{
		jmp     dword ptr [memicmpDispatch]
	}
}

__declspec(naked) static int __cdecl memicmpSSE2(const void *buffer1, const void *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    ebp
		push    esi
		push    edi
		xor     ebp, ebp                                    // ebp = 0
		mov     esi, dword ptr [buffer1 + 16]               // esi = buffer1
		mov     edi, dword ptr [buffer2 + 16]               // edi = buffer2
		mov     ebx, dword ptr [count + 16]                 // ebx = count
		sub     ebp, edi                                    // ebp = -buffer2
		add     esi, ebx                                    // esi = end of buffer1
		add     edi, ebx                                    // edi = end of buffer2
		xor     ebx, -1                                     // ebx = -count - 1
		and     ebp, 15                                     // ebp = -buffer2 & 15
		xor     eax, eax                                    // eax = 0
		jmp     byte_loop_entry

		align   16
	byte_loop:
		movzx   eax, byte ptr [esi + ebx]
		movzx   edx, byte ptr [edi + ebx]
		sub     eax, 'A'
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A' + 1
		lea     ecx, [eax + 'a' - 'A']
		cmovb   eax, ecx
		cmp     edx, 'Z' - 'A' + 1
		lea     ecx, [edx + 'a' - 'A']
		cmovb   edx, ecx
		sub     eax, edx
		jnz     epilogue
	byte_loop_entry:
		inc     ebx
		jz      epilogue
		sub     ebp, 1
		jae     byte_loop
		movdqa  xmm4, xmmword ptr [upper]
		movdqa  xmm5, xmmword ptr [azrange]
		movdqa  xmm6, xmmword ptr [casebit]                 // bit to change
		mov     edx, 15
		sub     esi, 15
		sub     edi, 15
		add     ebx, 15
		jnc     xmmword_loop
		sub     ebx, 15

		align   16
	xmmword_loop_last:
		mov     ecx, ebx
		add     esi, 15
		add     ecx, esi
		add     edi, 15
		shl     ecx, 32 - PAGE_SHIFT
		xor     edx, edx
		cmp     ecx, -15 shl (32 - PAGE_SHIFT)
		jae     byte_loop                                   // jump if cross pages

		align   16
	xmmword_loop:
		movdqu  xmm0, xmmword ptr [esi + ebx]               // load 16 byte
		movdqa  xmm1, xmmword ptr [edi + ebx]               //
		movdqa  xmm2, xmm0                                  // copy
		movdqa  xmm3, xmm1                                  //
		paddb   xmm0, xmm4                                  // all bytes greater than 'Z' if negative
		paddb   xmm1, xmm4                                  //
		pcmpgtb xmm0, xmm5                                  // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pcmpgtb xmm1, xmm5                                  //
		pand    xmm0, xmm6                                  // assign a mask for the appropriate bytes
		pand    xmm1, xmm6                                  //
		por     xmm0, xmm2                                  // negation of the 5th bit - lowercase letters
		por     xmm1, xmm3                                  //
		pcmpeqb xmm0, xmm1                                  // compare
		pmovmskb ecx, xmm0                                  // get one bit for each byte result
		xor     ecx, 0FFFFH
		jnz     xmmword_not_equal
		add     ebx, 16
		jnc     xmmword_loop
		sub     ebx, edx
		jb      xmmword_loop_last
		jmp     epilogue

		align   16
	xmmword_not_equal:
		bsf     ecx, ecx
		sub     ebx, edx
		add     esi, edx
		add     ebx, ecx
		jc      epilogue
		add     edi, edx
		xor     edx, edx
		mov     al, byte ptr [esi + ebx]
		mov     dl, byte ptr [edi + ebx]
		sub     eax, 'A'
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A' + 1
		lea     ecx, [eax + 'a' - 'A']
		cmovb   eax, ecx
		cmp     edx, 'Z' - 'A' + 1
		lea     ecx, [edx + 'a' - 'A']
		cmovb   edx, ecx
		sub     eax, edx
	epilogue:
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef buffer1
		#undef buffer2
		#undef count
	}
}

__declspec(naked) static int __cdecl memicmp386(const void *buffer1, const void *buffer2, size_t count)
{
	__asm
	{
		#define buffer1 (esp + 4)
		#define buffer2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax                                    // eax = 0
		mov     esi, dword ptr [buffer1 + 12]               // esi = buffer1
		mov     edi, dword ptr [buffer2 + 12]               // edi = buffer2
		mov     ecx, dword ptr [count + 12]                 // ecx = count
		xor     edx, edx                                    // edx = 0
		add     esi, ecx                                    // esi = end of buffer1
		add     edi, ecx                                    // edi = end of buffer2
		xor     ecx, -1                                     // ecx = -count - 1

		align   16
	loop_begin:
		inc     ecx
		jz      epilogue
		mov     al, byte ptr [esi + ecx]
		mov     dl, byte ptr [edi + ecx]
		sub     eax, edx
		jz      loop_begin
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a' + 1
		jb      loop_begin
		dec     eax
		jmp     epilogue

		align   16
	compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A' + 1
		jb      loop_begin
		inc     eax
		jmp     epilogue

		align   16
	not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
		cmp     eax, 'Z' - 'A' + 1
		jae     secondary_to_lower
		add     eax, 'a' - 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A' + 1
		jae     difference
		add     edx, 'a' - 'A'
	difference:
		sub     eax, edx
	epilogue:
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef buffer1
		#undef buffer2
		#undef count
	}
}

__declspec(naked) static int __cdecl memicmpCPUDispatch(const void *buffer1, const void *buffer2, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [memicmpDispatch], offset memicmp386
		jmp     memicmp386

	L1:
		mov     dword ptr [memicmpDispatch], offset memicmpSSE2
		jmp     memicmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
