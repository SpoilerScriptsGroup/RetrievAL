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

	iWidth = 0;
	if (WinControl &&
		WinControl->FFont &&
		(FResource = WinControl->FFont->FResource) &&
		(hWnd = TWinControl_GetHandle(WinControl)) &&
		(hDC = GetDC(WinControl->FHandle)))
	{
		HFONT hOldFont;

		if (!FResource->Font.Handle)
			FResource->Font.Handle = CreateFontA(
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
		if (FResource->Font.Handle &&
			(hOldFont = SelectObject(hDC, FResource->Font.Handle)))
		{
			SIZE size;

			if (GetTextExtentPoint32A(hDC, string_c_str(s), string_length(s), &size))
				iWidth = size.cx;
			SelectObject(hDC, hOldFont);
		}
		ReleaseDC(WinControl->FHandle, hDC);
	}
	return iWidth;
}
