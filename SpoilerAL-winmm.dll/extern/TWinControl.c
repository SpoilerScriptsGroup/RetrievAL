#define USING_NAMESPACE_BCB6_STD
#include "TWinControl.h"

__declspec(naked) HWND __fastcall TWinControl_GetHandle(LPCVOID WinControl)
{
	__asm
	{
		mov     edx, 0058750CH
		mov     eax, ecx
		jmp     edx
	}
}

int __fastcall TWinControl_GetTextWidth(TWinControl *WinControl, const string *s)
{
	int       iWidth;
	TResource *FResource;
	HWND      hWnd;
	HDC       hDC;

	if (!WinControl || !s)
		return 0;
	if (WinControl->FParentFont)
		return TWinControl_GetTextWidth(WinControl->FParent, s);
	iWidth = 0;
	if (WinControl->FFont &&
		(FResource = WinControl->FFont->FResource) &&
		(hWnd = TWinControl_GetHandle(WinControl)) &&
		(hDC = GetDC(hWnd)))
	{
		HFONT hFont;

		hFont = CreateFontA(
			FResource->Font.Height ?
				FResource->Font.Height < 0 ?
					FResource->Font.Height - 1 :
					FResource->Font.Height + 1 :
				FResource->Font.Height,
			0,
			0,
			0,
			!FResource->Font.Style.fsBold ? FW_NORMAL : FW_BOLD,
			FResource->Font.Style.fsItalic,
			FResource->Font.Style.fsUnderline,
			FResource->Font.Style.fsStrikeOut,
			FResource->Font.Charset,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			FResource->Font.Pitch,
			FResource->Font.Name.Data);
		if (hFont)
		{
			HFONT hOldFont;

			if (hOldFont = SelectObject(hDC, hFont))
			{
				SIZE size;

				if (GetTextExtentPoint32A(hDC, string_c_str(s), string_length(s), &size))
					iWidth = size.cx;
				SelectObject(hDC, hOldFont);
			}
			DeleteObject(hFont);
		}
		ReleaseDC(hWnd, hDC);
	}
	return iWidth;
}
