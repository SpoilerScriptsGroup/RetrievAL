#include "TMainForm.h"

#pragma warning(disable:4733)

void(__cdecl * const TMainForm_GoCalcEnter)(TMainForm *this) = (LPVOID)0x0043F454;
void(__cdecl * const TMainForm_DrawTreeCell)(TMainForm *this, LPVOID DestCanvas, int ARow, RECT *Rect) = (LPVOID)0x00444848;
void(__cdecl * const TMainForm_ChangeSubjectPanel)(TMainForm *this, long Type) = (LPVOID)0x004465AC;
void(__cdecl * const TMainForm_GoCalcHexChange)(TMainForm *this, boolean IsCalcHex) = (LPVOID)0x00447194;

__declspec(naked) void __fastcall TMainForm_StringEnterBtnClick(TMainForm *this, LPVOID Sender)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0043E98CH
		jmp     ecx
	}
}

__declspec(naked) void __stdcall TMainForm_Guide(const char *Mes, BOOLEAN IsClear)
{
	extern const DWORD F005D54CC;
	extern const DWORD F0048C1F8;
	extern const DWORD F0056DDBC;

	__asm
	{
		push    ebp
		mov     eax, 0060E730H
		mov     ebp, esp
		sub     esp, 36
		push    ebx
		mov     ebx, dword ptr ds:[_MainForm]
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
