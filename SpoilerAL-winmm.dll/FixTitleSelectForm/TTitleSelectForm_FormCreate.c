#include <windows.h>
#include <assert.h>
#define USING_NAMESPACE_BCB6_STD
#include "TTitleSelectForm.h"
#include "TFont.h"

__declspec(naked) void __cdecl TTitleSelectForm_FormCreate()
{
	static TTitleSelectForm * __fastcall FormCreate(TTitleSelectForm *this);

	#define _TTitleSelectForm_FormCreate 0x004714E8

	__asm
	{
		push    _TTitleSelectForm_FormCreate
		mov     ecx, eax
		jmp     FormCreate
	}

	#undef _TTitleSelectForm_FormCreate
}

static TTitleSelectForm * __fastcall FormCreate(TTitleSelectForm *this)
{
	TFont_Assign(this->FindLBox->FCanvas->FFont, this->FindLBox->FFont);
	TFont_Assign(this->KeyDGrid->FCanvas->FFont, this->KeyDGrid->FFont);
	TFont_Assign(this->TitleDGrid->FCanvas->FFont, this->TitleDGrid->FFont);
	return this;
}
