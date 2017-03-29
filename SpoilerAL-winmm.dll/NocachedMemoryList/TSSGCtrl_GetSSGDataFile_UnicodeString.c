#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

EXTERN_C void * __cdecl A_memcpy(void *dest, const void *src, size_t count);

__declspec(naked) bcb6_std_string * __cdecl TSSGCtrl_GetSimpleByteCode_unless_Unicode(bcb6_std_string *Result, TSSGCtrl *_this, TSSGSubject *SSGS, bcb6_std_string EndWord)
{
	/*
	if (bcb6_std_string_length(&EndWord) != 7 ||
		*(LPDWORD) EndWord._M_start      != BSWAP32('unic') ||
		*(LPDWORD)(EndWord._M_start + 4) != BSWAP32('ode\0'))
	{
		return TSSGCtrl_GetSimpleByteCode(Result, _this, SSGS, EndWord);
	}
	else
	{
		bcb6_std_string_dtor(&EndWord);
		bcb6_std_string_ctor(Result);
		return Result;
	}
	*/
	__asm
	{
		#define _TSSGCtrl_GetSimpleByteCode 00506BACH
		#define _bcb6_std_string_ctor       00402590H
		#define Result  (esp +  4)
		#define EndWord (esp + 16)

		mov     eax, dword ptr [EndWord]
		mov     ecx, dword ptr [EndWord + 4]
		sub     ecx, eax
		mov     edx, _TSSGCtrl_GetSimpleByteCode
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
		push    _bcb6_std_string_ctor
		jmp     bcb6_std_string_dtor
		align   16
	L1:
		jmp     edx
		align   16
	L2:
		pop     ecx
		pop     eax
		ret

		#undef _TSSGCtrl_GetSimpleByteCode
		#undef _bcb6_std_string_ctor
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

		mov     eax, dword ptr [EndCode]
		mov     ecx, dword ptr [EndCode + 4]
		cmp     eax, ecx
		je      L1
		jmp     A_memcpy
		align   16
	L1:
		; WideCharToMultiByte(CP_ACP, 0, src, count / 2, dest, count, NULL, NULL);
		; return dest;
		mov     edx, dword ptr [dest]
		mov     ecx, dword ptr [src]
		mov     eax, dword ptr [count]
		push    0
		push    0
		push    eax
		shr     eax, 1
		push    edx
		push    eax
		push    ecx
		push    0
		push    CP_ACP
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
	bcb6_std_string *Src,
	bcb6_std_string Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option)
{
	/*
	return !bcb6_std_string_empty(&Token) ?
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
