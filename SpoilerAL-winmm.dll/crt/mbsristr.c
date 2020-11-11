#include <windows.h>
#include <intrin.h>

#ifndef _WIN64
#pragma intrinsic(_subborrow_u32)
#define _sub_uintptr(a, b, out) _subborrow_u32(0, a, b, out)
#else
#pragma intrinsic(_subborrow_u64)
#define _sub_uintptr(a, b, out) _subborrow_u64(0, a, b, out)
#endif

#pragma function(strlen)

unsigned char * __fastcall internal_mbschr_single_byte(const unsigned char *string, unsigned int c);
unsigned char * __fastcall internal_mbschr_multi_byte(const unsigned char *string, unsigned int c);
unsigned char * __fastcall internal_mbsichr_single_byte(const unsigned char *string, unsigned int c);
unsigned char * __fastcall internal_mbsichr_multi_byte(const unsigned char *string, unsigned int c);

#ifndef _M_IX86
unsigned char * __cdecl _mbsristr(const unsigned char *haystack, const unsigned char *needle)
{
	unsigned char *(__fastcall * internal_mbsichr)(const unsigned char *string, unsigned int c);
	size_t        haystacklen, needlelen, difference, clen;
	unsigned char *result, *p, *end, low;
	LCID          locale;
	WORD          wCharType;
	unsigned int  c;

	needlelen = strlen((const char *)needle);
	if (!needlelen)
		return (unsigned char *)haystack;
	haystacklen = strlen((const char *)haystack);
	if (_sub_uintptr(haystacklen, needlelen, &difference))
		return NULL;
	result = NULL;
	end = (p = (unsigned char *)haystack) + difference;
	locale = GetThreadLocale();
	if (!IsDBCSLeadByteEx(CP_THREAD_ACP, c = needle[0]))
	{
		end += clen = 1;
		if ((c | ('a' - 'A')) - 'a' >= 'z' - 'a' + 1)
			internal_mbsichr = internal_mbschr_single_byte;
		else
			internal_mbsichr = internal_mbsichr_single_byte;
	}
	else
	{
		if (!(low = needle[1]))
			return NULL;
		c = (c << 8) | low;
		end += clen = 2;
		if (!GetStringTypeA(locale, CT_CTYPE1, needle, 2, &wCharType) || !(wCharType & (C1_UPPER | C1_LOWER)))
			internal_mbsichr = internal_mbschr_multi_byte;
		else
			internal_mbsichr = internal_mbsichr_multi_byte;
	}
	do
		if (!(p = internal_mbsichr(p, c)) || p >= end)
			break;
		else if (CompareStringA(locale, NORM_IGNORECASE, p, needlelen, needle, needlelen) == CSTR_EQUAL)
			result = p;
	while ((p += clen) < end);
	return result;
}
#else
__declspec(naked) unsigned char * __cdecl _mbsristr(const unsigned char *haystack, const unsigned char *needle)
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
		mov     bl, byte ptr [edi]
		lea     ebp, [eax + esi]
		call    GetThreadLocale
		mov     dword ptr [esp + 4], 0
		push    eax
		push    ebx
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jnz     L3
		mov     eax, ebx
		push    offset internal_mbschr_single_byte
		or      eax, 'a' - 'A'
		mov     edi, 1
		sub     eax, 'a'
		inc     ebp
		cmp     eax, 'z' - 'a' + 1
		jae     L4
		mov     dword ptr [esp], offset internal_mbsichr_single_byte
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
		mov     edx, dword ptr [needle + 28]
		mov     ecx, dword ptr [esp]
		push    offset internal_mbschr_multi_byte
		xor     eax, eax
		mov     al, byte ptr [edi + 1]
		mov     edi, 2
		test    eax, eax
		jz      L6
		shl     ebx, 8
		push    0
		or      ebx, eax
		push    esp
		push    edi
		push    edx
		push    CT_CTYPE1
		push    ecx
		call    GetStringTypeA
		pop     ecx
		add     ebp, edi
		test    eax, eax
		jz      L4
		and     ecx, C1_UPPER or C1_LOWER
		jz      L4
		mov     dword ptr [esp], offset internal_mbsichr_multi_byte

		align   16
	L4:
		mov     ecx, esi
		mov     edx, ebx
		call    dword ptr [esp]
		test    eax, eax
		jz      L6
		cmp     eax, ebp
		jae     L6
		mov     esi, eax
		mov     edx, dword ptr [needle + 32]
		mov     ecx, dword ptr [esp + 8]
		mov     eax, dword ptr [esp + 4]
		push    ecx
		push    edx
		push    ecx
		push    esi
		push    NORM_IGNORECASE
		push    eax
		call    CompareStringA
		cmp     eax, CSTR_EQUAL
		jne     L5
		mov     dword ptr [esp + 12], esi
	L5:
		add     esi, edi
		cmp     esi, ebp
		jb      L4
	L6:
		mov     eax, dword ptr [esp + 12]
		add     esp, 16
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
