#define USING_NAMESPACE_BCB6_STD
#include "TSSGSubject.h"

#pragma warning(disable:4733)

void(__cdecl *TSSGSubject_GetSubjectName)(string *Result, TSSGSubject *this, struct _TSSGCtrl *SSGC) = (LPVOID)0x0052CF6C;

extern const DWORD F005D54CC;
extern const DWORD TSSGScriptStruct_GetTitleTextWidth_CtorData[];

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
