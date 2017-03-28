#include <windows.h>
#include "bcb6_std_string.h"
#include "TSSGCtrl.h"
#include "TSSGSubject.h"

EXTERN_C void * __cdecl A_memcpy(void *dest, const void *src, size_t count);

bcb6_std_string * __cdecl TSSGCtrl_GetSimpleByteCode_unless_Unicode(bcb6_std_string *EndCode, TSSGCtrl *_this, TSSGSubject *SSGS, bcb6_std_string EndWord)
{
	if (bcb6_std_string_length(&EndWord) != 7 ||
		*(LPDWORD)EndWord._M_start       != BSWAP32('unic') ||
		*(LPDWORD)(EndWord._M_start + 4) != BSWAP32('ode\0'))
	{
		return TSSGCtrl_GetSimpleByteCode(EndCode, _this, SSGS, EndWord);
	}
	else
	{
		bcb6_std_string_dtor(&EndWord);
		bcb6_std_string_ctor(EndCode);
		return EndCode;
	}
}

__declspec(naked) char* __cdecl TSSGCtrl_GetSSGDataFile_CopyOrMapping(void *dest, const void *src, size_t count)
{
	__asm
	{
		#define EndCode (ebp - 11CH)

		mov     eax, dword ptr [EndCode]
		mov     ecx, dword ptr [EndCode + 4]
		cmp     eax, ecx
		je      L1
		jmp     A_memcpy
		align   16
	L1:
		; WideCharToMultiByte(CP_ACP, 0, src, count / 2, dest, count, NULL, NULL);
		mov     edx, dword ptr [esp +  4]
		mov     ecx, dword ptr [esp +  8]
		mov     eax, dword ptr [esp + 12]
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
		mov     eax, dword ptr [esp +  4]
		ret

		#undef EndCode
	}
}

unsigned long __cdecl TStringDivision_Find_unless_TokenIsEmpty(
	TStringDivision *_this,
	bcb6_std_string *Src,
	bcb6_std_string Token,
	unsigned long   FromIndex,
	unsigned long   ToIndex,
	unsigned long   Option)
{
	return !bcb6_std_string_empty(&Token) ?
		TStringDivision_Find(_this, Src, Token, FromIndex, ToIndex, Option) :
		-1;
}
