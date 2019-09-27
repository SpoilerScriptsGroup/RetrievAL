#include <stddef.h>
#include <ctype.h>
#include "PageSize.h"

#ifndef _M_IX86
int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	int ret, c;

	string1 += count;
	string2 += count;
	count ^= -1;
	while (++count)
		if (ret = tolower(string1[count]) - (c = tolower(string2[count])))
			return ret;
		else if (!c)
			break;
	return 0;
}
#else
static int __cdecl strnicmpSSE2(const char *string1, const char *string2, size_t count);
static int __cdecl strnicmp386(const char *string1, const char *string2, size_t count);
static int __cdecl strnicmpCPUDispatch(const char *string1, const char *string2, size_t count);

static int(__cdecl * strnicmpDispatch)(const char *string1, const char *string2, size_t count) = strnicmpCPUDispatch;

extern const char xmm_ahighA[16];
extern const char xmm_azrangeA[16];
extern const char xmm_casebitA[16];
#define ahigh   xmm_ahighA
#define azrange xmm_azrangeA
#define casebit xmm_casebitA

__declspec(naked) int __cdecl _strnicmp(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		jmp     dword ptr [strnicmpDispatch]
	}
}

__declspec(naked) static int __cdecl strnicmpSSE2(const char *string1, const char *string2, size_t count)
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
		xor     eax, eax
		add     esi, ebp                                // esi = end of string1
		add     edi, ebp                                // edi = end of string2
		xor     ebp, -1                                 // ebp = -count - 1
		xor     edx, edx
		movdqa  xmm4, xmmword ptr [ahigh]
		movdqa  xmm5, xmmword ptr [azrange]
		pxor    xmm6, xmm6                              // set to zero
		movdqa  xmm7, xmmword ptr [casebit]             // bit to change
		jmp     byte_loop_increment

		align   16
	byte_loop:
		mov     al, byte ptr [esi + ebp]
		mov     dl, byte ptr [edi + ebp]
		sub     eax, edx
		jnz     byte_compare_insensitive
		test    edx, edx
		jz      return_equal
	byte_loop_increment:
		inc     ebp
		jz      return_equal
		lea     ebx, [edi + ebp]
		lea     ecx, [esi + ebp]
		and     ebx, 15
		jnz     byte_loop
		and     ecx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	byte_compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      byte_compare_above
		cmp     eax, 'A' - 'a'
		jne     byte_not_equal
		xor     eax, eax
		lea     ecx, [edx - 'a']
		cmp     ecx, 'z' - 'a'
		jbe     byte_loop_increment
		sub     edx, 'A'
		mov     eax, ecx
		jmp     secondary_to_lower

		align   16
	byte_compare_above:
		xor     eax, eax
		lea     ecx, [edx - 'A']
		cmp     ecx, 'Z' - 'A'
		jbe     byte_loop_increment
		mov     edx, ecx
		lea     eax, [ecx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	byte_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
		jmp     secondary_to_lower

		align   16
	xmmword_loop:
		cmp     ecx, PAGE_SIZE - 15
		ja      byte_loop                                // jump if cross pages
		movdqu  xmm3, xmmword ptr [esi + ebp]            // enter 16 byte
		movdqa  xmm1, xmmword ptr [edi + ebp]            //
		movdqa  xmm0, xmm3                               // copy
		pcmpeqb xmm3, xmm6                               // compare 16 bytes with zero
		movdqa  xmm2, xmm0                               // copy
		pmovmskb ecx, xmm3                               // get one bit for each byte result
		movdqa  xmm3, xmm1                               // copy
		psubb   xmm0, xmm4                               // all bytes less than 'A'
		psubb   xmm1, xmm4                               //
		psubusb xmm0, xmm5                               // and 'Z' will be reset
		psubusb xmm1, xmm5                               //
		pcmpeqb xmm0, xmm6                               // xmm0 = (byte >= 'A' && byte <= 'Z') ? 0xFF : 0x00
		pcmpeqb xmm1, xmm6                               //
		pand    xmm0, xmm7                               // assign a mask for the appropriate bytes
		pand    xmm1, xmm7                               //
		pxor    xmm0, xmm2                               // negation of the 5th bit - lowercase letters
		pxor    xmm1, xmm3                               //
		pcmpeqb xmm0, xmm1                               // compare
		pmovmskb ebx, xmm0                               // get one bit for each byte result
		xor     ebx, 0FFFFH
		jnz     xmmword_not_equal
		test    ecx, ecx
		jnz     return_equal
		add     ebp, 16
		jc      return_equal
		lea     ecx, [esi + ebp]
		and     ecx, PAGE_SIZE - 1
		jmp     xmmword_loop

		align   16
	xmmword_not_equal:
		test    ecx, ecx
		jz      xmmword_has_not_null
		bsf     ecx, ecx
		mov     edx, 0FFFFH
		xor     ecx, 15
		shr     edx, cl
		and     ebx, edx
		jz      return_equal
		xor     edx, edx
	xmmword_has_not_null:
		bsf     ebx, ebx
		add     ebp, ebx
		jc      return_equal
		mov     dl, byte ptr [edi + ebp]
		mov     al, byte ptr [esi + ebp]
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

		#undef string1
		#undef string2
		#undef count
	}
}

__declspec(naked) static int __cdecl strnicmp386(const char *string1, const char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		push    edi
		xor     eax, eax
		mov     esi, dword ptr [string1 + 12]
		mov     edi, dword ptr [string2 + 12]
		mov     ecx, dword ptr [count + 12]
		xor     edx, edx
		lea     esi, [esi + ecx]
		lea     edi, [edi + ecx]
		xor     ecx, -1

		align   16
	loop_begin:
		inc     ecx
		jz      return_equal
		mov     al, byte ptr [esi + ecx]
		mov     dl, byte ptr [edi + ecx]
		sub     eax, edx
		jnz     compare_insensitive
		test    edx, edx
		jnz     loop_begin
	return_equal:
		pop     edi
		pop     esi
		pop     ebx
		ret

		align   16
	compare_insensitive:
		cmp     eax, 'a' - 'A'
		je      compare_above
		cmp     eax, 'A' - 'a'
		jne     return_not_equal
		xor     eax, eax
		lea     ebx, [edx - 'a']
		cmp     ebx, 'z' - 'a'
		jbe     loop_begin
		sub     edx, 'A'
		mov     eax, ebx
		jmp     secondary_to_lower

		align   16
	compare_above:
		xor     eax, eax
		lea     ebx, [edx - 'A']
		cmp     ebx, 'Z' - 'A'
		jbe     loop_begin
		mov     edx, ebx
		lea     eax, [ebx + 'a' - 'A']
		jmp     primary_to_lower

		align   16
	return_not_equal:
		lea     eax, [eax + edx - 'A']
		sub     edx, 'A'
	secondary_to_lower:
		cmp     edx, 'Z' - 'A'
		ja      primary_to_lower
		add     edx, 'a' - 'A'
	primary_to_lower:
		cmp     eax, 'Z' - 'A'
		ja      difference
		add     eax, 'a' - 'A'
	difference:
		sub     eax, edx
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}

__declspec(naked) static int __cdecl strnicmpCPUDispatch(const char *string1, const char *string2, size_t count)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strnicmpDispatch], offset strnicmp386
		jmp     strnicmp386
	L1:
		mov     dword ptr [strnicmpDispatch], offset strnicmpSSE2
		jmp     strnicmpSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
