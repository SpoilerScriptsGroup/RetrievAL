#define USING_NAMESPACE_BCB6_STD
#include "TCanvas.h"

extern const DWORD vcl_System_AnsiString_ctor_sz;
extern const DWORD vcl_System_AnsiString_dtor;

#pragma function(strlen)

#define CSHANDLE 0x01
#define CSFONT   0x02
#define CSPen    0x04
#define CSBRUSH  0x08

__declspec(naked) void __fastcall TCanvas_FillRect(TCanvas *this, const RECT *rect)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055E1D4H
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TCanvas_Lock(TCanvas *this)
{
	__asm
	{
		mov     edx, 0055E284H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) int __fastcall TCanvas_TextWidth(TCanvas *this, LPCSTR Text)
{
	extern const DWORD F0055E820;

	__asm
	{
		push    edx
		push    ecx
		push    edx
		mov     dl, CSHANDLE or CSFONT
		mov     eax, ecx
		call    dword ptr [F0055E820]
		call    strlen
		pop     edx
		pop     ecx
		pop     edx
		mov     ecx, dword ptr [ecx + 4]
		push    0
		push    0
		push    esp
		push    eax
		push    edx
		push    ecx
		call    GetTextExtentPoint32A
		pop     eax
		pop     edx
		ret
	}
}

__declspec(naked) int __fastcall TCanvas_TextWidth_with_length(TCanvas *this, LPCSTR Text, size_t length)
{
	extern const DWORD F0055E820;

	__asm
	{
		#define length (esp + 4)

		push    edx
		push    ecx
		mov     dl, CSHANDLE or CSFONT
		mov     eax, ecx
		call    dword ptr [F0055E820]
		pop     ecx
		pop     edx
		mov     ecx, dword ptr [ecx + 4]
		mov     eax, dword ptr [length]
		push    0
		push    0
		push    esp
		push    eax
		push    edx
		push    ecx
		call    GetTextExtentPoint32A
		pop     eax
		pop     edx
		ret     4

		#undef length
	}
}

__declspec(naked) int __fastcall TCanvas_TextWidth_stdstr(TCanvas *this, const string *Text)
{
	extern const DWORD F0055E820;

	__asm
	{
		push    ecx
		push    edx
		mov     dl, CSHANDLE or CSFONT
		mov     eax, ecx
		call    dword ptr [F0055E820]
		pop     edx
		pop     ecx
		mov     eax, dword ptr [edx + 4]
		mov     edx, dword ptr [edx]
		mov     ecx, dword ptr [ecx + 4]
		sub     eax, edx
		push    0
		push    0
		push    esp
		push    eax
		push    edx
		push    ecx
		call    GetTextExtentPoint32A
		pop     eax
		pop     edx
		ret
	}
}

__declspec(naked) void __fastcall TCanvas_Unlock(TCanvas *this)
{
	__asm
	{
		mov     edx, 0055E620H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) void __fastcall TCanvas_SetFont(TCanvas *this, TFont *Font)
{
	__asm
	{
		mov     eax, dword ptr [ecx + 12]
		mov     ecx, dword ptr [eax]
		jmp     dword ptr [ecx + 8]
	}
}

__declspec(naked) void __fastcall TCanvas_SetPen(TCanvas *this, TPen *Pen)
{
	__asm
	{
		mov     eax, dword ptr [ecx + 16]
		mov     ecx, dword ptr [eax]
		jmp     dword ptr [ecx + 8]
	}
}

__declspec(naked) void __fastcall TCanvas_SetBrush(TCanvas *this, TBrush *Brush)
{
	__asm
	{
		mov     eax, dword ptr [ecx + 20]
		mov     ecx, dword ptr [eax]
		jmp     dword ptr [ecx + 8]
	}
}

__declspec(naked) HDC __fastcall TCanvas_GetHandle(TCanvas *this)
{
	__asm
	{
		mov     edx, 0055E74CH
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) void __stdcall TCanvas_TextOut(TCanvas *this, int X, int Y, LPCSTR Text)
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
