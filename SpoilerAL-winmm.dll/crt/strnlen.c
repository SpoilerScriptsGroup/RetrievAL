#include <string.h>

#ifndef _M_IX86
size_t __cdecl _tcsnlen(const char *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		while (*(string++) && --length);
		length = maxlen - length;
	}
	return length;
}
#else
static size_t __cdecl strnlenSSE2(const char *string, size_t maxlen);
static size_t __cdecl strnlen386(const char *string, size_t maxlen);
static size_t __cdecl strnlenCPUDispatch(const char *string, size_t maxlen);

static size_t(__cdecl * strnlenDispatch)(const char *string, size_t maxlen) = strnlenCPUDispatch;

__declspec(naked) size_t __cdecl strnlen(const char *string, size_t maxlen)
{
	__asm
	{
		jmp     dword ptr [strnlenDispatch]
	}
}

__declspec(naked) static size_t __cdecl strnlenSSE2(const char *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov         eax, dword ptr [maxlen]
		mov         ecx, dword ptr [string]
		test        eax, eax
		jz          L1
		push        ebx
		mov         ebx, ecx
		and         ecx, 15
		and         ebx, -16
		pxor        xmm0, xmm0
		movdqa      xmm1, xmmword ptr [ebx]
		pcmpeqb     xmm1, xmm0
		pmovmskb    edx, xmm1
		shr         edx, cl
		xor         ecx, -1
		test        edx, edx
		jz          L2
		bsf         ecx, edx
		cmp         eax, ecx
		pop         ebx
		cmova       eax, ecx
	L1:
		ret

		align       16
	L2:
		add         ecx, 17
		add         ebx, 16
		sub         ecx, eax
		jae         L4
		sub         ebx, ecx

		align       16
	L3:
		movdqa      xmm1, xmmword ptr [ecx + ebx]
		pcmpeqb     xmm1, xmm0
		pmovmskb    edx, xmm1
		test        edx, edx
		jnz         L5
		add         ecx, 16
		jnc         L3
	L4:
		pop         ebx
		ret

		align       16
	L5:
		bsf         edx, edx
		add         ecx, edx
		pop         ebx
		add         ecx, eax
		cmovc       eax, ecx
		ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl strnlen386(const char *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]
		mov     edx, dword ptr [string]
		test    eax, eax
		jz      L3
		push    ebx
		mov     ebx, eax
		mov     ecx, edx
		and     edx, -4
		and     ecx, 3
		jz      L1
		shl     ecx, 3
		mov     eax, dword ptr [edx]
		shr     eax, cl
		xor     ecx, -1
		add     ecx, 33
		or      ebx, -1
		shl     ebx, cl
		add     edx, 4
		shr     ecx, 3
		or      eax, ebx
		lea     ebx, [eax - 01010101H]
		xor     eax, -1
		and     eax, ebx
		mov     ebx, dword ptr [maxlen + 4]
		and     eax, 80808080H
		jnz     L4
		sub     ebx, ecx
		jbe     L2

		align   16
	L1:
		mov     eax, dword ptr [edx]
		add     edx, 4
		lea     ecx, [eax - 01010101H]
		xor     eax, -1
		and     ecx, 80808080H
		and     eax, ecx
		jnz     L4
		sub     ebx, 4
		ja      L1
	L2:
		mov     eax, dword ptr [maxlen + 4]
		pop     ebx
	L3:
		ret

		align   16
	L4:
		mov     ecx, eax
		mov     edx, ebx
		mov     eax, dword ptr [maxlen + 4]
		pop     ebx
		test    cx, cx
		jnz     L5
		sub     edx, 2
		jbe     L6
		shr     ecx, 16
	L5:
		inc     eax
		add     cl, cl
		sbb     eax, edx
	L6:
		ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl strnlenCPUDispatch(const char *string, size_t maxlen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [strnlenDispatch], offset strnlen386
		jmp     strnlen386
	L1:
		mov     dword ptr [strnlenDispatch], offset strnlenSSE2
		jmp     strnlenSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}
#endif
