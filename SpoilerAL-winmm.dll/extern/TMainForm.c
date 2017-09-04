#include "TMainForm.h"

#pragma warning(disable:4733)

void(__cdecl *TMainForm_DrawTreeCell)(TMainForm *this, LPVOID DestCanvas, int ARow, RECT *Rect) = (LPVOID)0x00444848;
void(__cdecl *TMainForm_ChangeSubjectPanel)(TMainForm *this, long Type) = (LPVOID)0x004465AC;

extern const DWORD F005D54CC;
extern const DWORD F0048C1F8;
extern const DWORD F0056DDBC;

__declspec(naked) void __fastcall TMainForm_HotKeyEditKeyDown(TMainForm *this, LPVOID Sender, WORD *Key, int Shift)
{
	__asm
	{
		mov     eax, dword ptr [esp]
		push    ecx
		mov     ecx, dword ptr [esp + 8]
		mov     dword ptr [esp + 4], 00443054H
		mov     dword ptr [esp + 8], eax
		pop     eax
		ret
	}
}

__declspec(naked) void __stdcall TMainForm_Guide(const char *Mes, BOOLEAN IsClear)
{
	__asm
	{
		push    ebp
		mov     eax, 0064CE2CH
		mov     ebp, esp
		sub     esp, 36
		push    ebx
		mov     ebx, dword ptr [eax]
		mov     eax, 0060E730H
		call    dword ptr [F005D54CC]
		mov     ecx, dword ptr [ebx + 448H]
		mov     eax, dword ptr [ebx + 56CH]
		dec     ecx
		jz      L2
		test    eax, eax
		jnz     L1
		mov     ecx, ebx
		mov     eax, 0062089CH
		mov     dl, 1
		mov     eax, dword ptr [eax]
		call    dword ptr [F0048C1F8]
		mov     dword ptr [ebx + 56CH], eax
	L1:
		mov     ecx, dword ptr [ebp + 0CH]
		mov     edx, dword ptr [ebp + 8H]
		push    ecx
		push    edx
		push    eax
		call    TGuideForm_Guide
		mov     eax, dword ptr [ebx + 56CH]
		add     esp, 12
		cmp     byte ptr [eax + 57H], 0
		jne     L2
		call    dword ptr [F0056DDBC]
	L2:
		mov     ecx, dword ptr [ebp - 36]
		mov     dword ptr fs:[0], ecx
		pop     ebx
		mov     esp, ebp
		pop     ebp
		ret     8
	}
}
