#include <windows.h>
#define USING_NAMESPACE_BCB6_STD
#include "TFindNameForm.h"

extern const DWORD _TFindNameForm_FormClose;

__declspec(naked) void __cdecl TFindNameForm_FormClose()
{
	static TFindNameForm * __fastcall FormClose(TFindNameForm *this);

	__asm
	{
		push    ecx
		mov     ecx, eax
		call    FormClose
		pop     ecx
		jmp     dword ptr [_TFindNameForm_FormClose]
	}
}

static TFindNameForm * __fastcall FormClose(TFindNameForm *this)
{
	TResource *Resource;
	HFONT     hFont;

	if (hFont = (Resource = this->FindLBox->FFont->FResource)->Font.Handle)
	{
		Resource->Font.Handle = NULL;
		DeleteObject(hFont);
	}
	return this;
}
