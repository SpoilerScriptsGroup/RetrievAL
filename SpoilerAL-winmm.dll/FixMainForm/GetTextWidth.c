#define USING_NAMESPACE_BCB6_STD
#include "TWinControl.h"
#include "bcb6_std_string.h"

int __fastcall GetTextWidth(TWinControl *WinControl, const string *s)
{
	HWND      hWnd;
	HDC       hDC;
	int       iWidth;
	TResource *Resource;
	HFONT     hOldFont;

	if (!WinControl)
		return 0;
	if (WinControl->FParentFont)
		return GetTextWidth(WinControl->FParent, s);
	if (!s || !(hWnd = TWinControl_GetHandle(WinControl)) || !(hDC = GetDC(hWnd)))
		return 0;
	iWidth = 0;
	if (!(Resource = WinControl->FFont->FResource)->Font.Handle)
	{
		LOGFONTA lf;

		lf.lfHeight         = Resource->Font.Height;
		lf.lfWidth          = 0;
		lf.lfEscapement     = 0;
		lf.lfOrientation    = 0;
		lf.lfWeight         = !Resource->Font.Style.fsBold ? FW_NORMAL : FW_BOLD;
		lf.lfItalic         = Resource->Font.Style.fsItalic;
		lf.lfUnderline      = Resource->Font.Style.fsUnderline;
		lf.lfStrikeOut      = Resource->Font.Style.fsStrikeOut;
		lf.lfCharSet        = Resource->Font.Charset;
		lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
		lf.lfQuality        = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = Resource->Font.Pitch;
		memcpy(lf.lfFaceName, Resource->Font.Name.Data, Resource->Font.Name.Len);
		lf.lfFaceName[Resource->Font.Name.Len] = '\0';
		if (!(Resource->Font.Handle = CreateFontIndirectA(&lf)))
			if (!SystemParametersInfoA(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0) ||
				!(Resource->Font.Handle = CreateFontIndirectA(&lf)))
				Resource->Font.Handle = GetStockObject(SYSTEM_FONT);
	}
	if (Resource->Font.Handle && (hOldFont = SelectObject(hDC, Resource->Font.Handle)))
	{
		SIZE size;

		if (GetTextExtentPoint32A(hDC, string_c_str(s), string_length(s), &size))
			iWidth = size.cx;
		SelectObject(hDC, hOldFont);
	}
	ReleaseDC(hWnd, hDC);
	return iWidth;
}
