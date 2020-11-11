#include <windows.h>
#include <intrin.h>

#ifndef _WIN64
#pragma intrinsic(_subborrow_u32)
#define _sub_uintptr(a, b, out) _subborrow_u32(0, a, b, out)
#else
#pragma intrinsic(_subborrow_u64)
#define _sub_uintptr(a, b, out) _subborrow_u64(0, a, b, out)
#endif

#pragma function(strlen, memcmp)

unsigned char * __fastcall internal_mbschr_single_byte(const unsigned char *string, unsigned int c);
unsigned char * __fastcall internal_mbschr_multi_byte(const unsigned char *string, unsigned int c);

#ifndef _M_IX86
unsigned char * __cdecl _mbsrstr(const unsigned char *haystack, const unsigned char *needle)
{
	unsigned char *(__fastcall * internal_mbschr)(const unsigned char *string, unsigned int c);
	size_t        haystacklen, needlelen, difference, clen;
	unsigned char *result, *p, *end, low;
	unsigned int  c;

	needlelen = strlen((const char *)needle);
	if (!needlelen)
		return (unsigned char *)haystack;
	haystacklen = strlen((const char *)haystack);
	if (_sub_uintptr(haystacklen, needlelen, &difference))
		return NULL;
	result = NULL;
	end = (p = (unsigned char *)haystack) + difference;
	if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c = needle[0]))
	{
		end += clen = 1;
		internal_mbschr = internal_mbschr_single_byte;
	}
	else
	{
		if (!(low = needle[1]))
			return NULL;
		c = (c << 8) | low;
		end += clen = 2;
		internal_mbschr = internal_mbschr_multi_byte;
	}
	do
		if (!(p = internal_mbschr(p, c)) || p >= end)
			break;
		else if (memcmp(p, needle, needlelen) == 0)
			result = p;
	while ((p += clen) < end);
	return result;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsrstr(const unsigned char *haystack, const unsigned char *needle)
{
	__asm
	{
		#define haystack (esp + 4)
		#define needle   (esp + 8)

		push    ebx
		push    ebp
		push    esi
		push    edi
		mov     esi, dword ptr [haystack + 16]
		mov     edi, dword ptr [needle + 16]
		push    edi
		call    strlen
		test    eax, eax
		jz      L1
		mov     dword ptr [esp], esi
		mov     ebp, eax
		call    strlen
		sub     eax, ebp
		jb      L2
		push    ebp
		xor     ebx, ebx
		mov     dword ptr [esp + 4], ebx
		sub     esp, 16
		mov     bl, byte ptr [edi]
		lea     ebp, [eax + esi]
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jnz     L3
		mov     dword ptr [esp + 12], offset internal_mbschr_single_byte
		mov     edi, 1
		inc     ebp
		jmp     L4

		align   16
	L1:
		mov     eax, esi
		pop     ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   8
	L2:
		xor     eax, eax
		pop     ecx
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		align   16
	L3:
		mov     dword ptr [esp + 12], offset internal_mbschr_multi_byte
		xor     eax, eax
		mov     al, byte ptr [edi + 1]
		mov     edi, 2
		test    eax, eax
		jz      L6
		shl     ebx, 8
		add     ebp, edi
		or      ebx, eax

		align   16
	L4:
		mov     ecx, esi
		mov     edx, ebx
		call    dword ptr [esp + 12]
		test    eax, eax
		jz      L6
		cmp     eax, ebp
		jae     L6
		mov     ecx, dword ptr [needle + 40]
		mov     edx, dword ptr [esp + 16]
		mov     esi, eax
		mov     dword ptr [esp], eax
		mov     dword ptr [esp + 4], ecx
		mov     dword ptr [esp + 8], edx
		call    memcmp
		test    eax, eax
		jnz     L5
		mov     dword ptr [esp + 20], esi
	L5:
		add     esi, edi
		cmp     esi, ebp
		jb      L4
	L6:
		mov     eax, dword ptr [esp + 20]
		add     esp, 24
		pop     edi
		pop     esi
		pop     ebp
		pop     ebx
		ret

		#undef haystack
		#undef needle
	}
}
#endif
