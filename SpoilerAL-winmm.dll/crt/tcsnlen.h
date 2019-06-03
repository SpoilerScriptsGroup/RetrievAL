#include <windows.h>
#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
#ifndef _MBCS
size_t __cdecl _tcsnlen(const TCHAR *string, size_t maxlen)
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
size_t __cdecl _mbsnlen(const unsigned char *string, size_t maxlen)
{
	size_t length;

	if (length = maxlen)
	{
		unsigned char c;

		while ((c = *(p++)) && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c) || *(p++)) && --length);
		length = maxlen - length;
	}
	return length;
}
#endif
#elif !defined(_MBCS) && !defined(_UNICODE)
static size_t __cdecl strnlen_initializer(const char *string, size_t maxlen);
static size_t __cdecl strnlen386(const char *string, size_t maxlen);
static size_t __cdecl strnlenSSE2(const char *string, size_t maxlen);

static size_t(__cdecl * _imp__strnlen)(const char *string, size_t maxlen) = strnlen_initializer;

__declspec(naked) size_t __cdecl strnlen(const char *string, size_t maxlen)
{
	__asm
	{
		jmp     dword ptr [_imp__strnlen]
	}
}

__declspec(naked) static size_t __cdecl strnlen_initializer(const char *string, size_t maxlen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [_imp__strnlen], offset strnlen386
		jmp     strnlen386
	L1:
		mov     dword ptr [_imp__strnlen], offset strnlenSSE2
		jmp     strnlenSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
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
		mov     ebx, -1
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
#elif !defined(_MBCS) && defined(_UNICODE)
static size_t __cdecl wcsnlen_initializer(const wchar_t *string, size_t maxlen);
static size_t __cdecl wcsnlen386(const wchar_t *string, size_t maxlen);
static size_t __cdecl wcsnlenSSE2(const wchar_t *string, size_t maxlen);

static size_t(__cdecl * _imp__wcsnlen)(const wchar_t *string, size_t maxlen) = wcsnlen_initializer;

__declspec(naked) size_t __cdecl wcsnlen(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		jmp     dword ptr [_imp__wcsnlen]
	}
}

__declspec(naked) static size_t __cdecl wcsnlen_initializer(const wchar_t *string, size_t maxlen)
{
	#define __ISA_AVAILABLE_X86  0
	#define __ISA_AVAILABLE_SSE2 1

	extern unsigned int __isa_available;

	__asm
	{
		cmp     dword ptr [__isa_available], __ISA_AVAILABLE_X86
		jne     L1
		mov     dword ptr [_imp__wcsnlen], offset wcsnlen386
		jmp     wcsnlen386
	L1:
		mov     dword ptr [_imp__wcsnlen], offset wcsnlenSSE2
		jmp     wcsnlenSSE2
	}

	#undef __ISA_AVAILABLE_X86
	#undef __ISA_AVAILABLE_SSE2
}

__declspec(naked) static size_t __cdecl wcsnlen386(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]
		mov     edx, dword ptr [string]
		test    eax, eax
		jz      L3

		align   16
	L1:
		mov     cx, word ptr [edx]
		add     edx, 2
		test    cx, cx
		jz      L2
		dec     eax
		jnz     L1
	L2:
		mov     ecx, eax
		mov     eax, dword ptr [maxlen]
		sub     eax, ecx
	L3:
		ret

		#undef string
		#undef maxlen
	}
}

__declspec(naked) static size_t __cdecl wcsnlenSSE2(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov         eax, dword ptr [maxlen]
		mov         ecx, dword ptr [string]
		test        eax, eax
		jz          L4
		mov         edx, ecx
		and         ecx, 15
		xor         edx, ecx
		pxor        xmm0, xmm0
		movdqa      xmm1, xmmword ptr [edx]
		test        ecx, 1
		jnz         L1
		pcmpeqw     xmm1, xmm0
		pmovmskb    eax, xmm1
		shr         eax, cl
		push        offset L6
		test        eax, eax
		jz          L3
		bsf         eax, eax
		pop         ecx
		jmp         L9

		align       16
	L1:
		pcmpeqb     xmm1, xmm0
		pmovmskb    eax, xmm1
		add         eax, 0x5555
		inc         ecx
		shr         eax, cl
		dec         edx
		and         eax, 0x1555
		jnz         L2
		push        offset L7
		jmp         L3

		align       16
	L2:
		bsf         eax, eax
		jmp         L9

		align       16
	L3:
		shr         ecx, 1
		mov         eax, 8
		sub         eax, ecx
		mov         ecx, dword ptr [maxlen + 4]
		sub         ecx, eax
		jbe         L5
		lea         edx, [edx + ecx * 2 + 16]
		lea         ecx, [ecx * 2 - 1]
		xor         ecx, -1
	L4:
		ret

		align       16
	L5:
		add         eax, ecx
		pop         ecx
		ret

		align       16
	L6:
		movdqa      xmm1, xmmword ptr [ecx + edx]
		pcmpeqw     xmm1, xmm0
		pmovmskb    eax, xmm1
		test        eax, eax
		jnz         L8
		add         ecx, 16
		jnc         L6
		mov         eax, dword ptr [maxlen]
		ret

		align       16
	L7:
		movdqu      xmm1, xmmword ptr [ecx + edx]
		pcmpeqw     xmm1, xmm0
		pmovmskb    eax, xmm1
		test        eax, eax
		jnz         L8
		add         ecx, 16
		jnc         L7
		mov         eax, dword ptr [maxlen]
		ret

		align       16
	L8:
		bsf         eax, eax
		add         eax, ecx
		mov         ecx, dword ptr [string]
		add         eax, edx
		sub         eax, ecx
	L9:
		shr         eax, 1
		mov         ecx, dword ptr [maxlen]
		cmp         eax, ecx
		cmova       eax, ecx
		ret

		#undef string
		#undef maxlen
	}
}
#else
__declspec(naked) size_t __cdecl _mbsnlen(const unsigned char *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]
		mov     ecx, dword ptr [string]
		test    eax, eax
		jz      L4
		push    ebx
		push    esi
		mov     ebx, eax
		xor     eax, eax
		mov     esi, ecx

		align   16
	L1:
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		xor     eax, eax
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jz      L3
	L2:
		dec     ebx
		jnz     L1
	L3:
		mov     eax, dword ptr [maxlen + 8]
		pop     esi
		sub     eax, ebx
		pop     ebx
	L4:
		ret

		#undef string
		#undef maxlen
	}
}
#endif
