#include "TCanvas.h"

extern const DWORD vcl_System_AnsiString_ctor_sz;
extern const DWORD vcl_System_AnsiString_dtor;

__declspec(naked) void __fastcall TCanvas_FillRect(LPVOID this, const RECT *rect)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055E1D4H
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TCanvas_Lock(LPVOID this)
{
	__asm
	{
		mov     edx, 0055E284H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) int __fastcall TCanvas_TextWidth(LPVOID this, LPCSTR Text)
{
	extern const DWORD _TCanvas_TextWidth;

	__asm
	{
		push    eax
		mov     eax, esp
		push    ecx
		call    dword ptr [vcl_System_AnsiString_ctor_sz]
		mov     edx, dword ptr [eax]
		pop     eax
		call    dword ptr [_TCanvas_TextWidth]
		push    eax
		mov     edx, 2
		lea     eax, [esp + 4]
		call    dword ptr [vcl_System_AnsiString_dtor]
		pop     eax
		pop     ecx
		ret
	}
}

__declspec(naked) void __fastcall TCanvas_Unlock(LPVOID this)
{
	__asm
	{
		mov     edx, 0055E620H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) HDC __fastcall TCanvas_GetHandle(LPVOID this)
{
	__asm
	{
		mov     edx, 0055E74CH
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) void __stdcall TCanvas_TextOut(LPVOID this, int X, int Y, LPCSTR Text)
{
	extern const DWORD _TCanvas_TextOut;

	__asm
	{
		#define this (esp +  4)
		#define X    (esp +  8)
		#define Y    (esp + 12)
		#define Text (esp + 16)

		mov     edx, dword ptr [Text]
		push    eax
		mov     eax, esp
		call    dword ptr [vcl_System_AnsiString_ctor_sz]
		mov     ecx, dword ptr [eax]
		push    ecx
		mov     ecx, dword ptr [Y + 4]
		mov     edx, dword ptr [X + 4]
		mov     eax, dword ptr [this + 4]
		call    dword ptr [_TCanvas_TextOut]
		mov     edx, 2
		mov     eax, esp
		call    dword ptr [vcl_System_AnsiString_dtor]
		pop     eax
		ret     16

		#undef this
		#undef X
		#undef Y
		#undef Text
	}
}
