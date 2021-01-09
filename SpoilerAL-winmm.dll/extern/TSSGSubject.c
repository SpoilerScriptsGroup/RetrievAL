#include "xx.h"
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
		NULL,// sentinel, dttFrameOffs = void *[2] + args shadow
	};
	static const EXCTAB ERRcXtab = {
		NULL, -(signed)(sizeof(REGREC_BC) + sizeof(LPVOID) * 3),
		XB_DEST << 16 | /* 'outer' */0, /* 'min dtor count' */0, (uintptr_t)&vt,
	};
	TSSGSubject *fake[2];
	__asm
	{
		test    ecx, ecx
		jz      L1

		push    ebp
		mov     ebp, esp
		add     esp, ERRcXtab.xtBPoffs
		
		mov     this, ecx
		mov     eax, offset ERRcXtab
		call    __InitExceptBlockLDTC
		mov     ecx, this

		mov     eax, size TSSGSubjectProperty
		mul     [ecx]TSSGSubject.propertyIndex
		jc      NO_PROP

		mov     edx, SubjectProperty
		mov     [edx + eax]TSSGSubjectProperty.RepeatDepth, MAXDWORD

	NO_PROP:
		mov     eax, [ecx]TSSGSubject.VTable
		mov     fake[1 * size LPVOID], eax// The second entry is the vtable pointer.
		mov     fake[0 * size LPVOID], ecx// The first entry of the array is the pointer to be deleted.
		mov     [esp]REGREC_BC.ERRcCCtx, 8// offset EXCTAB.xtTable

		push    DTCVF_PTRVAL | DTCVF_DELPTR
		push    ecx
		call    [eax]SubjectVtbl.dtor
		add     esp, 8

		mov     eax, [esp]REGREC_BC.ERRcNext
		mov     fs:[0], eax// __ExitExceptBlock

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
