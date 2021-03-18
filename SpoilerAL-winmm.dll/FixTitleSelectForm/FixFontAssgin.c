#include <windows.h>
#include "TTitleSelectForm.h"
#include "TFont.h"

void __fastcall FixFontAssgin(TTitleSelectForm *this)
{
	TFont_SetHeight(this->TitleDGrid->FFont, -16);
	TFont_SetStyle(this->TitleDGrid->FFont, 0);
	TFont_SetCharset(this->TitleDGrid->FFont, SHIFTJIS_CHARSET);
	TFont_SetPitch(this->TitleDGrid->FFont, 0);
	TFont_SetName(this->TitleDGrid->FFont, "‚l‚r ‚oƒSƒVƒbƒN");
	TFont_Assign(this->KeyDGrid->FFont, this->TitleDGrid->FFont);
	TFont_Assign(this->TitleDGrid->FCanvas->FFont, this->TitleDGrid->FFont);
	TFont_SetHeight(this->TitleDGrid->FCanvas->FFont, -17);
	TFont_Assign(this->KeyDGrid->FCanvas->FFont, this->KeyDGrid->FFont);
}