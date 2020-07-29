#include "rtl.h"
#define USING_NAMESPACE_BCB6_STD
#include "TSSGSubject.h"
#include "SSGSubjectProperty.h"

#pragma warning(disable:4733)

void(__cdecl * const TSSGSubject_GetSubjectName)(string *Result, TSSGSubject *this, struct _TSSGCtrl *SSGC) = (LPVOID)0x0052CF6C;

extern const DWORD __InitExceptBlockLDTC;
extern const DWORD TSSGScriptStruct_GetTitleTextWidth_CtorData[];

__declspec(naked) void __fastcall delete_TSSGSubject(TSSGSubject *this)
{
	static const struct DTT vt[] = {
		(void *)0x00462E80, 0x0000500F, -(signed)sizeof(LPVOID) * 3,
		NULL,// sentinel
	};
	static const EXCTAB ERRcXtab = {
		NULL, -(signed)(sizeof(REGREC_BC) + sizeof(LPVOID) * 3),
		XB_DEST << 16 | /*outer ctx*/0, /* 'min dtor count' */0, (uintptr_t)vt,
	};

	__asm
	{
		test    ecx, ecx
		jz      L1

		push    ebp
		mov     ebp, esp
		sub     esp, size REGREC_BC + size LPVOID * 3
		mov     this, ecx

		mov     eax, offset ERRcXtab
		call    __InitExceptBlockLDTC

		mov     ecx, this
		mov     eax, size TSSGSubjectProperty
		mul     dword ptr [ecx + TSSGSubject.propertyIndex]
		jc      NO_PROP

		mov     edx, SubjectProperty
		mov     dword ptr [edx + eax], MAXDWORD

	NO_PROP:
		mov     eax, dword ptr [ecx + TSSGSubject.VTable]
		mov     dword ptr [ebp - size LPVOID * 3], ecx// The first entry of the array is the pointer to be deleted.
		mov     dword ptr [ebp - size LPVOID * 2], eax// The second entry is the vtable pointer.
		mov      word ptr [esp + REGREC_BC.ERRcCCtx], 8// offset EXCTAB.xtTable

		push    DTCVF_PTRVAL | DTCVF_DELPTR
		push    ecx
		call    dword ptr [eax + TSSGSubjectVtbl.dtor]
		add     esp, 8

		mov     eax, dword ptr [esp + REGREC_BC.ERRcNext]
		mov     dword ptr fs:[0], eax// __ExitExceptBlock

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
		call    dword ptr [__InitExceptBlockLDTC]
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
