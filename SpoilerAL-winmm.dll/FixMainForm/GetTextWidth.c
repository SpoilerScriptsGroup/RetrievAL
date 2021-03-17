#define USING_NAMESPACE_BCB6_STD
#include "TWinControl.h"
#include "bcb6_std_string.h"

int __fastcall GetTextWidth(TWinControl *WinControl, const string *s)
{
	HWND      hWnd;
	HDC       hDC;
	TResource *Resource;
	LOGFONTA  lf;
	int       iWidth;
	HFONT     hFont;

	if (!WinControl)
		return 0;
	if (WinControl->FParentFont)
		return GetTextWidth(WinControl->FParent, s);
	if (!s || !(hWnd = TWinControl_GetHandle(WinControl)) || !(hDC = GetDC(hWnd)))
		return 0;
	Resource = WinControl->FFont->FResource;
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
	iWidth = 0;
	if (hFont = CreateFontIndirectA(&lf))
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
	return iWidth;
}
