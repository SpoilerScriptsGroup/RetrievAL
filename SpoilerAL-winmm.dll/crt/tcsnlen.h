#include <windows.h>
#include <string.h>
#include <tchar.h>

#ifndef _M_IX86
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
#elif defined(_UNICODE)
__declspec(naked) size_t __cdecl wcsnlen(const wchar_t *string, size_t maxlen)
{
	__asm
	{
		#define string (esp + 4)
		#define maxlen (esp + 8)

		mov     eax, dword ptr [maxlen]
		mov     edx, dword ptr [string]
		test    eax, eax
		jz      short L3

		align   16
	L1:
		mov     cx, word ptr [edx]
		add     edx, 2
		test    cx, cx
		jz      short L2
		dec     eax
		jnz     short L1
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
#else
static size_t __cdecl strnlen_initializer(const char *string, size_t maxlen);
static size_t __cdecl strnlen386(const char *string, size_t maxlen);
static size_t __cdecl strnlenSSE2(const char *string, size_t maxlen);

static const void *_imp__strnlen = (const void *)strnlen_initializer;

__declspec(naked) size_t __cdecl strnlen(const char *string, size_t maxlen)
{
	__asm
	{
		jmp     dword ptr [_imp__strnlen]
	}
}

__declspec(naked) static size_t __cdecl strnlen_initializer(const char *string, size_t maxlen)
{
	__asm
	{
		push    PF_XMMI64_INSTRUCTIONS_AVAILABLE
		call    IsProcessorFeaturePresent
		test    eax, eax
		jnz     L1
		mov     dword ptr [_imp__strnlen], offset strnlen386
		jmp     strnlen386
	L1:
		mov     dword ptr [_imp__strnlen], offset strnlenSSE2
		jmp     strnlenSSE2
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
		jz      short L3
		mov     dword ptr [esp + 4], ebx
		mov     ebx, eax
		mov     eax, edx
		mov     ecx, 4
		and     eax, 3
		jz      short L1
		sub     ecx, eax
		xor     edx, eax
		mov     eax, -1
		push    ecx
		shl     ecx, 3
		push    ebx
		shr     eax, cl
		mov     ebx, dword ptr [edx]
		add     edx, 4
		or      eax, ebx
		rol     eax, cl
		pop     ebx
		lea     ecx, [eax - 01010101H]
		xor     eax, -1
		and     eax, ecx
		pop     ecx
		and     eax, 80808080H
		jnz     short L4
		sub     ebx, ecx
		jbe     short L2

		align   16
	L1:
		mov     eax, dword ptr [edx]
		add     edx, 4
		lea     ecx, [eax - 01010101H]
		xor     eax, -1
		and     ecx, 80808080H
		and     eax, ecx
		jnz     short L4
		sub     ebx, 4
		ja      short L1
	L2:
		mov     eax, dword ptr [maxlen]
		mov     ebx, dword ptr [esp + 4]
	L3:
		ret

		align   16
	L4:
		bsf     ecx, eax
		shr     ecx, 3
		mov     eax, dword ptr [maxlen]
		sub     ebx, ecx
		jbe     short L5
		sub     eax, ebx
	L5:
		mov     ebx, dword ptr [esp + 4]
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
		jz          short L3
		mov         edx, ecx
		and         ecx, 15
		xor         edx, ecx
		pxor        xmm0, xmm0
		movdqa      xmm1, xmmword ptr [edx]
		pcmpeqb     xmm1, xmm0
		pmovmskb    eax, xmm1
		shr         eax, cl
		jnz         short L4
		mov         ecx, dword ptr [string]
		lea         eax, [edx + 16]
		sub         eax, ecx
		mov         ecx, dword ptr [maxlen]
		sub         ecx, eax
		jbe         short L2
		lea         edx, [ecx + edx + 16]
		dec         ecx
		xor         ecx, -1

		align       16
	L1:
		movdqa      xmm1, xmmword ptr [ecx + edx]
		pcmpeqb     xmm1, xmm0
		pmovmskb    eax, xmm1
		test        eax, eax
		jnz         short L4
		add         ecx, 16
		jnc         short L1
	L2:
		mov         eax, dword ptr [maxlen]
	L3:
		ret

		align       16
	L4:
		bsf         eax, eax
		add         eax, edx
		mov         edx, dword ptr [string]
		add         eax, ecx
		mov         ecx, dword ptr [maxlen]
		sub         eax, edx
		cmp         eax, ecx
		cmova       eax, ecx
		ret

		#undef string
		#undef maxlen
	}
}
#endif
