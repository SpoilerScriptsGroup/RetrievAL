#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"
#include "TranscodeMultiByte.h"

#pragma function(memcpy)

#define _BSWAP32(value) (            \
    (((value) >> 24) & 0x000000FF) | \
    (((value) >>  8) & 0x0000FF00) | \
    (((value) <<  8) & 0x00FF0000) | \
    (((value) << 24) & 0xFF000000))

__declspec(naked) string * __cdecl TSSGCtrl_GetSimpleByteCode_unless_Unicode(string *Result, TSSGCtrl *this, TSSGSubject *SSGS, string EndWord)
{
	/*
	if ((string_length(&EndWord) == 7 &&
		*(LPDWORD) EndWord._M_start      == BSWAP32('unic') &&
		*(LPDWORD)(EndWord._M_start + 4) == BSWAP32('ode\0')) ||
		(string_length(&EndWord) == 4 &&
		*(LPDWORD) EndWord._M_start      == BSWAP32('utf8')))
	{
		string_dtor(&EndWord);
		return string_ctor(Result);
	}
	else
	{
		return TSSGCtrl_GetSimpleByteCode(Result, this, SSGS, EndWord);
	}
	*/
	__asm
	{
		#define Result  (esp +  4)
		#define EndWord (esp + 16)

		mov     edx, dword ptr [EndWord]
		mov     eax, dword ptr [EndWord + 4]
		sub     eax, edx
		lea     ecx, [EndWord]
		cmp     eax, 7
		jne     L1
		mov     eax, dword ptr [edx]
		mov     edx, dword ptr [edx + 4]
		cmp     eax, _BSWAP32('unic')
		jne     L3
		cmp     edx, _BSWAP32('ode\0')
		je      L2
		jmp     L3
	L1:
		cmp     eax, 4
		jne     L3
		cmp     dword ptr [edx], _BSWAP32('utf8')
		jne     L3
	L2:
		call    string_dtor
		mov     ecx, dword ptr [Result]
		jmp     string_ctor

		align   16
	L3:
		jmp     dword ptr [TSSGCtrl_GetSimpleByteCode]

		#undef Result
		#undef EndWord
	}
}

__declspec(naked) char* __cdecl TSSGCtrl_GetSSGDataFile_CopyOrMapping(void *dest, const void *src, size_t count)
{
	/*
		if (!EndCode.empty())
			memcpy(tmpC, SIt, StrSize);
		else if (EndWord != "utf8")
			WideCharToMultiByte(CP_THREAD_ACP, 0, src, (count + 2) >> 1, dest, count + 1, NULL, NULL);
		else
			Utf8ToMultiByte(CP_THREAD_ACP, 0, src, count + 1, dest, count + 1, NULL, NULL);
		return dest;
	*/
	__asm
	{
		#define EndCode (ebp - 11CH)
		#define EndWord (ebp - 50H)
		#define dest    (esp +  4)
		#define src     (esp +  8)
		#define count   (esp + 12)

		mov     ecx, dword ptr [EndCode]
		mov     eax, dword ptr [EndCode + 4]
		cmp     eax, ecx
		je      L1
		jmp     memcpy

		align   16
	L1:
		mov     ecx, dword ptr [EndWord]
		mov     eax, dword ptr [EndWord + 4]
		sub     eax, ecx
		cmp     eax, 4
		jne     L2
		cmp     dword ptr [ecx], _BSWAP32('utf8')
		je      L3
	L2:
		mov     ecx, dword ptr [count]
		mov     edx, dword ptr [dest]
		inc     ecx
		push    0
		push    0
		push    ecx
		push    edx
		inc     ecx
		shr     ecx, 1
		mov     edx, dword ptr [src + 16]
		push    ecx
		push    edx
		push    0
		push    CP_THREAD_ACP
		call    WideCharToMultiByte
		mov     eax, dword ptr [dest]
		ret

		align   16
	L3:
		mov     ecx, dword ptr [count]
		mov     edx, dword ptr [dest]
		inc     ecx
		push    0
		push    0
		push    ecx
		push    edx
		mov     edx, dword ptr [src + 16]
		push    0
		push    CP_THREAD_ACP
		push    ecx
		push    edx
		push    0
		push    CP_UTF8
		call    TranscodeMultiByte
		mov     eax, dword ptr [dest]
		ret

		#undef EndCode
		#undef EndWord
		#undef dest
		#undef src
		#undef count
	}
}

__declspec(naked) unsigned long __cdecl TStringDivision_Find_unless_TokenIsEmpty(
	TStringDivision *this,
	string          *Src,
	string          Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option)
{
	/*
	return !string_empty(&Token) ?
		TStringDivision_Find(this, Src, Token, FromIndex, ToIndex, Option) :
		-1;
	*/
	__asm
	{
		#define Token (esp + 12)

		mov     eax, dword ptr [Token + 4]
		sub     eax, dword ptr [Token]
		jz      L1
		jmp     TStringDivision_Find

		align   16
	L1:
		dec     eax
		ret

		#undef Token
	}
}
