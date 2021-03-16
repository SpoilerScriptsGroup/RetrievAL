#include <windows.h>
#include <assert.h>
#define USING_NAMESPACE_BCB6_STD
#include "TTitleSelectForm.h"

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
	this->TitleDGrid->FFont->FResource->Font.Height = -17;
	assert(this->KeyDGrid->FFont->FResource == this->TitleDGrid->FFont->FResource);
	return this;
}
