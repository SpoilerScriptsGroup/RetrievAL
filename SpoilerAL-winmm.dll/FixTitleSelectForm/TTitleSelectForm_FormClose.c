#include <windows.h>
#include <assert.h>
#define USING_NAMESPACE_BCB6_STD
#include "TTitleSelectForm.h"

__declspec(naked) void __cdecl TTitleSelectForm_FormClose()
{
	static TTitleSelectForm * __fastcall FormClose(TTitleSelectForm *this);

	#define _TTitleSelectForm_FormClose 0x004715E4

	__asm
	{
		push    _TTitleSelectForm_FormClose
		mov     ecx, eax
		jmp     FormClose
	}

	#undef _TTitleSelectForm_FormClose
}

static TTitleSelectForm * __fastcall FormClose(TTitleSelectForm *this)
{
	TResource *Resource;
	HFONT     hFont;

	assert(this->KeyDGrid->FFont->FResource == this->TitleDGrid->FFont->FResource);
	if (hFont = (Resource = this->TitleDGrid->FFont->FResource)->Font.Handle)
	{
		Resource->Font.Handle = NULL;
		DeleteObject(hFont);
	}
	if (hFont = (Resource = this->FindLBox->FFont->FResource)->Font.Handle)
	{
		Resource->Font.Handle = NULL;
		DeleteObject(hFont);
	}
	return this;
}
