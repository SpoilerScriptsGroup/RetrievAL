#include "TFont.h"

extern const DWORD vcl_System_AnsiString_ctor_sz;
extern const DWORD vcl_System_AnsiString_dtor;

__declspec(naked) void __fastcall TFont_Assign(TFont *this, TFont *Source)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055D214H
		jmp     ecx
	}
}

__declspec(naked) void __fastcall TFont_SetColor(TFont *this, TColor Color)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055D350H
		jmp     ecx
	}
}

__declspec(naked) int __fastcall TFont_GetHeight(TFont *this)
{
	__asm
	{
		mov     edx, 0055D530H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) void __fastcall TFont_SetHeight(TFont *this, int Height)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055D538H
		jmp     ecx
	}
}

LPCSTR __fastcall TFont_GetName(TFont *this)
{
	static char lfFaceName[LF_FACESIZE];

	memcpy(lfFaceName, this->FResource->Font.Name.Data, this->FResource->Font.Name.Len);
	lfFaceName[this->FResource->Font.Name.Len] = '\0';
	return lfFaceName;
}

__declspec(naked) void __fastcall TFont_SetName(TFont *this, LPCSTR Name)
{
	extern const DWORD _TFont_SetName;

	__asm
	{
		push    eax
		mov     eax, esp
		push    ecx
		call    dword ptr [vcl_System_AnsiString_ctor_sz]
		mov     edx, dword ptr [eax]
		pop     eax
		call    dword ptr [_TFont_SetName]
		mov     edx, 2
		mov     eax, esp
		call    dword ptr [vcl_System_AnsiString_dtor]
		pop     eax
		ret
	}
}

__declspec(naked) int __fastcall TFont_GetSize(TFont *this)
{
	__asm
	{
		mov     edx, 0055D5D4H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) void __fastcall TFont_SetSize(TFont *this, int Size)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055D5F0H
		jmp     ecx
	}
}

__declspec(naked) int __fastcall TFont_GetStyle(TFont *this)
{
	__asm
	{
		mov     edx, 0055D610H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) void __fastcall TFont_SetStyle(TFont *this, TFontStyles Style)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055D61CH
		jmp     ecx
	}
}

__declspec(naked) TFontPitch __fastcall TFont_GetPitch(TFont *this)
{
	__asm
	{
		mov     edx, 0055D648H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) void __fastcall TFont_SetPitch(TFont *this, int Pitch)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055D650H
		jmp     ecx
	}
}

__declspec(naked) TFontCharset __fastcall TFont_GetCharset(TFont *this)
{
	__asm
	{
		mov     edx, 0055D678H
		mov     eax, ecx
		jmp     edx
	}
}

__declspec(naked) void __fastcall TFont_SetCharset(TFont *this, TFontCharset Charset)
{
	__asm
	{
		mov     eax, ecx
		mov     ecx, 0055D680H
		jmp     ecx
	}
}
