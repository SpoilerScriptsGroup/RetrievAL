#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

#pragma function(memcpy)

__declspec(naked) string * __cdecl TSSGCtrl_GetSimpleByteCode_unless_Unicode(string *Result, TSSGCtrl *_this, TSSGSubject *SSGS, string EndWord)
{
	/*
	if (string_length(&EndWord) != 7 ||
		*(LPDWORD) EndWord._M_start      != BSWAP32('unic') ||
		*(LPDWORD)(EndWord._M_start + 4) != BSWAP32('ode\0'))
	{
		return TSSGCtrl_GetSimpleByteCode(Result, _this, SSGS, EndWord);
	}
	else
	{
		string_dtor(&EndWord);
		string_ctor(Result);
		return Result;
	}
	*/
	__asm
	{
		#define _string_ctor 00402590H
		#define Result  (esp +  4)
		#define EndWord (esp + 16)

		mov     eax, dword ptr [EndWord]
		mov     ecx, dword ptr [EndWord + 4]
		sub     ecx, eax
		cmp     ecx, 7
		jne     L1
		mov     ecx, dword ptr [eax]
		mov     eax, dword ptr [eax + 4]
		cmp     ecx, 'cinu'
		jne     L1
		cmp     eax, 'edo'
		jne     L1
		mov     eax, dword ptr [Result]
		lea     ecx, [EndWord]
		push    eax
		push    eax
		push    offset L2
		push    _string_ctor
		jmp     string_dtor
		align   16
	L1:
		jmp     dword ptr [TSSGCtrl_GetSimpleByteCode]
		align   16
	L2:
		pop     ecx
		pop     eax
		ret

		#undef _string_ctor
		#undef Result
		#undef EndWord
	}
}

__declspec(naked) char* __cdecl TSSGCtrl_GetSSGDataFile_CopyOrMapping(void *dest, const void *src, size_t count)
{
	__asm
	{
		#define EndCode (ebp - 11CH)
		#define dest    (esp +  4)
		#define src     (esp +  8)
		#define count   (esp + 12)

		mov     ecx, dword ptr [EndCode]
		mov     eax, dword ptr [EndCode + 4]
		sub     eax, ecx
		jz      L1
		jmp     memcpy
		align   16
	L1:
		; WideCharToMultiByte(CP_THREAD_ACP, 0, src, count / 2, dest, count, NULL, NULL);
		; return dest;
		mov     ecx, dword ptr [count]
		mov     edx, dword ptr [dest]
		push    eax
		push    eax
		push    ecx
		push    edx
		shr     ecx, 1
		mov     edx, dword ptr [src + 16]
		push    ecx
		push    edx
		push    eax
		push    eax
		call    WideCharToMultiByte
		mov     eax, dword ptr [dest]
		ret

		#undef EndCode
		#undef dest
		#undef src
		#undef count
	}
}

__declspec(naked) unsigned long __cdecl TStringDivision_Find_unless_TokenIsEmpty(
	TStringDivision *_this,
	string          *Src,
	string          Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option)
{
	/*
	return !string_empty(&Token) ?
		TStringDivision_Find(_this, Src, Token, FromIndex, ToIndex, Option) :
		-1;
	*/
	__asm
	{
		#define Token (esp + 12)

		mov     ecx, dword ptr [Token]
		mov     eax, dword ptr [Token + 4]
		sub     eax, ecx
		jz      L1
		jmp     TStringDivision_Find
		align   16
	L1:
		dec     eax
		ret

		#undef Token
	}
}
