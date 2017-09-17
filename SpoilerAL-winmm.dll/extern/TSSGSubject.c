#define USING_NAMESPACE_BCB6_STD
#include "TSSGSubject.h"

#pragma warning(disable:4733)

void(__cdecl *TSSGSubject_GetSubjectName)(string *Result, TSSGSubject *this, struct _TSSGCtrl *SSGC) = (LPVOID)0x0052CF6C;

extern const DWORD F005D54CC;
extern const DWORD TSSGScriptStruct_GetTitleTextWidth_CtorData[];

__declspec(naked) void __fastcall delete_TSSGSubject(TSSGSubject *SSGS)
{
	static const DWORD data1[] = {
		0x00000008, 0x00100400,
		0x0045C094,
		0x00000002,
		BSWAP32('TSSG'),
		BSWAP32('Subj'),
		BSWAP32('ect '),
		BSWAP32('*[2]'),
		BSWAP32('\0\0\0\0'),
	};
	static const DWORD data2[] = {
		(DWORD)data1,
		0x0000500F, -8,
		0x00000000,
	};
	static const DWORD data3[] = {
		0x00000000, -44,
		0x00050000, 0x00000000,
		0x00000000, 0x00050008,
		0x00000000,
		(DWORD)data2,
	};

	__asm
	{
		test    ecx, ecx
		jz      L1
		push    ebp
		mov     eax, offset data3
		mov     ebp, esp
		sub     esp, 44
		mov     dword ptr [ebp - 8], ecx
		call    dword ptr [F005D54CC]
		mov     ecx, dword ptr [ebp - 8]
		push    3
		mov     eax, dword ptr [ecx]
		push    ecx
		mov     dword ptr [ebp - 4], eax
		call    dword ptr [eax]
		mov     ecx, dword ptr [ebp - 44]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
	L1:
		ret
	}
}

__declspec(naked) long __stdcall TSSGSubject_GetSubjectNameTextWidth(TSSGSubject *this, HDC hDC, struct _TSSGCtrl *SSGCtrl)
{
	__asm
	{
		push    ebp
		mov     eax, offset TSSGScriptStruct_GetTitleTextWidth_CtorData
		mov     ebp, esp
		sub     esp, 68
		call    dword ptr [F005D54CC]
		mov     ecx, dword ptr [ebp + 10H]
		mov     eax, dword ptr [ebp + 8H]
		push    ecx
		lea     ecx, [ebp - 18H]
		push    eax
		push    ecx
		call    dword ptr [TSSGSubject_GetSubjectName]
		add     esp, 12
		lea     eax, [ebp - 44H]
		mov     ecx, dword ptr [ebp - 18H]
		mov     edx, dword ptr [ebp - 14H]
		sub     edx, ecx
		push    eax
		mov     eax, dword ptr [ebp + 0CH]
		push    edx
		push    ecx
		push    eax
		call    GetTextExtentPoint32A
		lea     ecx, [ebp - 18H]
		call    string_dtor
		mov     ecx, dword ptr [ebp - 60]
		mov     eax, dword ptr [ebp - 44H]
		mov     dword ptr fs:[0], ecx
		mov     esp, ebp
		pop     ebp
		ret     12
	}
}
